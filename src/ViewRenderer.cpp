#include "ViewRenderer.h"

#include <iostream>
#include <math.h>

#include "ClassicDefs.h"
#include "Map.h"
#include "Player.h"

ViewRenderer::ViewRenderer() : m_UseClassicDoomScreenToAngle(false) {}

ViewRenderer::~ViewRenderer() {}

void ViewRenderer::init(Map *pMap, Player *pPlayer) {
	m_pMap = pMap;
	m_pPlayer = pPlayer;

	m_iRenderXSize = 320 * 2;
	m_iRenderYSize = 200 * 2;

	m_HalfScreenWidth = m_iRenderXSize / 2;
	m_HalfScreenHeight = m_iRenderYSize / 2;
	Angle HalfFOV = m_pPlayer->getFOV() / 2;
	m_iDistancePlayerToScreen = m_HalfScreenWidth / HalfFOV.getTanValue();

	for (int i = 0; i <= m_iRenderXSize; ++i) {
		if (m_UseClassicDoomScreenToAngle) {
			m_ScreenXToAngle[i] = classicDoomScreenXtoView[i];
		} else {
			m_ScreenXToAngle[i] = atan((m_HalfScreenWidth - i) / (float) m_iDistancePlayerToScreen) * 180 / M_PI;
		}
	}

	m_CeilingClipHeight.resize(m_iRenderXSize);
	m_FloorClipHeight.resize(m_iRenderXSize);
}

void ViewRenderer::render(uint8_t *pScreenBuffer, int iBufferPitch) {
	m_pScreenBuffer = pScreenBuffer;
	m_iBufferPitch = iBufferPitch;

	initFrame();
	render3DView();
}

void ViewRenderer::initFrame() {
	m_SolidWallRanges.clear();

	SolidSegmentRange WallLeftSide;
	SolidSegmentRange WallRightSide;

	WallLeftSide.XStart = INT_MIN;
	WallLeftSide.XEnd = -1;
	m_SolidWallRanges.push_back(WallLeftSide);

	WallRightSide.XStart = m_iRenderXSize;
	WallRightSide.XEnd = INT_MAX;
	m_SolidWallRanges.push_back(WallRightSide);

	std::fill(m_CeilingClipHeight.begin(), m_CeilingClipHeight.end(), -1);
	std::fill(m_FloorClipHeight.begin(), m_FloorClipHeight.end(), m_iRenderYSize);
}

void ViewRenderer::addWallInFOV(Seg &seg, Angle V1Angle, Angle V2Angle, Angle V1AngleFromPlayer, Angle V2AngleFromPlayer) {
	// Find Wall X Coordinates
	int V1XScreen = angleToScreen(V1AngleFromPlayer);
	int V2XScreen = angleToScreen(V2AngleFromPlayer);

	// Skip same pixel wall
	if (V1XScreen == V2XScreen) return;

	// Handle solid walls
	if (seg.pLeftSector == nullptr) {
		clipSolidWalls(seg, V1XScreen, V2XScreen, V1Angle, V2Angle);
		return;
	}

	// Handle closed door
	if (seg.pLeftSector->CeilingHeight <= seg.pRightSector->FloorHeight ||
	    seg.pLeftSector->FloorHeight >= seg.pRightSector->CeilingHeight) {
		clipSolidWalls(seg, V1XScreen, V2XScreen, V1Angle, V2Angle);
		return;
	}

	// windowed walls
	if (seg.pRightSector->CeilingHeight != seg.pLeftSector->CeilingHeight ||
	    seg.pRightSector->FloorHeight != seg.pLeftSector->FloorHeight) {
		clipPassWalls(seg, V1XScreen, V2XScreen, V1Angle, V2Angle);
		return;
	}
}

void ViewRenderer::clipSolidWalls(Seg &seg, int V1XScreen, int V2XScreen, Angle V1Angle, Angle V2Angle) {
	if (m_SolidWallRanges.size() < 2) {
		return;
	}
	// Find clip window
	SolidSegmentRange CurrentWall = {V1XScreen, V2XScreen};

	std::list<SolidSegmentRange>::iterator FoundClipWall = m_SolidWallRanges.begin();
	while (FoundClipWall != m_SolidWallRanges.end() && FoundClipWall->XEnd < CurrentWall.XStart - 1) {
		++FoundClipWall;
	}

	if (CurrentWall.XStart < FoundClipWall->XStart) {
		if (CurrentWall.XEnd < FoundClipWall->XStart - 1) {
			// All of the wall is visible, so insert it
			storeWallRange(seg, CurrentWall.XStart, CurrentWall.XEnd, V1Angle, V2Angle);
			m_SolidWallRanges.insert(FoundClipWall, CurrentWall);
			return;
		}

		// The end is already included, just update start
		storeWallRange(seg, CurrentWall.XStart, FoundClipWall->XStart - 1, V1Angle, V2Angle);
		FoundClipWall->XStart = CurrentWall.XStart;
	}

	// This part is already occupied
	if (CurrentWall.XEnd <= FoundClipWall->XEnd) return;

	std::list<SolidSegmentRange>::iterator NextWall = FoundClipWall;

	while (CurrentWall.XEnd >= next(NextWall, 1)->XStart - 1) {
		// partialy clipped by other walls, store each fragment
		storeWallRange(seg, NextWall->XEnd + 1, next(NextWall, 1)->XStart - 1, V1Angle, V2Angle);
		++NextWall;

		if (CurrentWall.XEnd <= NextWall->XEnd) {
			FoundClipWall->XEnd = NextWall->XEnd;
			if (NextWall != FoundClipWall) {
				FoundClipWall++;
				NextWall++;
				m_SolidWallRanges.erase(FoundClipWall, NextWall);
			}
			return;
		}
	}

	storeWallRange(seg, NextWall->XEnd + 1, CurrentWall.XEnd, V1Angle, V2Angle);
	FoundClipWall->XEnd = CurrentWall.XEnd;

	if (NextWall != FoundClipWall) {
		FoundClipWall++;
		NextWall++;
		m_SolidWallRanges.erase(FoundClipWall, NextWall);
	}
}

void ViewRenderer::clipPassWalls(Seg &seg, int V1XScreen, int V2XScreen, Angle V1Angle, Angle V2Angle) {
	// Find clip window
	SolidSegmentRange CurrentWall = {V1XScreen, V2XScreen};

	std::list<SolidSegmentRange>::iterator FoundClipWall = m_SolidWallRanges.begin();
	while (FoundClipWall != m_SolidWallRanges.end() && FoundClipWall->XEnd < CurrentWall.XStart - 1) {
		++FoundClipWall;
	}

	if (CurrentWall.XStart < FoundClipWall->XStart) {
		if (CurrentWall.XEnd < FoundClipWall->XStart - 1) {
			// All of the wall is visible, so insert it
			storeWallRange(seg, CurrentWall.XStart, CurrentWall.XEnd, V1Angle, V2Angle);
			return;
		}

		// The end is already included, just update start
		storeWallRange(seg, CurrentWall.XStart, FoundClipWall->XStart - 1, V1Angle, V2Angle);
	}

	// This part is already occupied
	if (CurrentWall.XEnd <= FoundClipWall->XEnd) return;

	std::list<SolidSegmentRange>::iterator NextWall = FoundClipWall;

	while (CurrentWall.XEnd >= next(NextWall, 1)->XStart - 1) {
		// partialy clipped by other walls, store each fragment
		storeWallRange(seg, NextWall->XEnd + 1, next(NextWall, 1)->XStart - 1, V1Angle, V2Angle);
		++NextWall;

		if (CurrentWall.XEnd <= NextWall->XEnd) {
			return;
		}
	}

	storeWallRange(seg, NextWall->XEnd + 1, CurrentWall.XEnd, V1Angle, V2Angle);
}

void ViewRenderer::storeWallRange(Seg &seg, int V1XScreen, int V2XScreen, Angle V1Angle, Angle V2Angle) {
	calculateWallHeight(seg, V1XScreen, V2XScreen, V1Angle, V2Angle);
	// calculateWallHeightSimple(seg, V1XScreen, V2XScreen, V1Angle, V2Angle);
}

void ViewRenderer::calculateWallHeight(Seg &seg, int V1XScreen, int V2XScreen, Angle V1Angle, Angle V2Angle) {
	// Calculate the distance to the first edge of the wall
	Angle             Angle90(90);
	SegmentRenderData RenderData{0};
	Angle             SegToNormalAngle = seg.SlopeAngle + Angle90;

	// Angle NomalToV1Angle = abs(SegToNormalAngle.GetSignedValue() - V1Angle.GetSignedValue());
	Angle NomalToV1Angle = SegToNormalAngle.getValue() - V1Angle.getValue();

	// Normal angle is 90 degree to wall
	Angle SegToPlayerAngle = Angle90 - NomalToV1Angle;

	RenderData.V1XScreen = V1XScreen;
	RenderData.V2XScreen = V2XScreen;
	RenderData.V1Angle = V1Angle;
	RenderData.V2Angle = V2Angle;

	RenderData.DistanceToV1 = m_pPlayer->distanceToPoint(*seg.pStartVertex);
	RenderData.DistanceToNormal = SegToPlayerAngle.getSinValue() * RenderData.DistanceToV1;

	RenderData.V1ScaleFactor = getScaleFactor(V1XScreen, SegToNormalAngle, RenderData.DistanceToNormal);
	RenderData.V2ScaleFactor = getScaleFactor(V2XScreen, SegToNormalAngle, RenderData.DistanceToNormal);

	RenderData.Steps = (RenderData.V2ScaleFactor - RenderData.V1ScaleFactor) / (V2XScreen - V1XScreen);

	RenderData.RightSectorCeiling = seg.pRightSector->CeilingHeight - m_pPlayer->getZPosition();
	RenderData.RightSectorFloor = seg.pRightSector->FloorHeight - m_pPlayer->getZPosition();

	RenderData.CeilingStep = -(RenderData.RightSectorCeiling * RenderData.Steps);
	RenderData.CeilingEnd = round(m_HalfScreenHeight - (RenderData.RightSectorCeiling * RenderData.V1ScaleFactor));

	RenderData.FloorStep = -(RenderData.RightSectorFloor * RenderData.Steps);
	RenderData.FloorStart = round(m_HalfScreenHeight - (RenderData.RightSectorFloor * RenderData.V1ScaleFactor));

	RenderData.pSeg = &seg;

	if (seg.pLeftSector) {
		RenderData.LeftSectorCeiling = seg.pLeftSector->CeilingHeight - m_pPlayer->getZPosition();
		RenderData.LeftSectorFloor = seg.pLeftSector->FloorHeight - m_pPlayer->getZPosition();

		ceilingFloorUpdate(RenderData);

		if (RenderData.LeftSectorCeiling < RenderData.RightSectorCeiling) {
			RenderData.bDrawUpperSection = true;
			RenderData.UpperHeightStep = -(RenderData.LeftSectorCeiling * RenderData.Steps);
			RenderData.iUpperHeight =
			    round(m_HalfScreenHeight - (RenderData.LeftSectorCeiling * RenderData.V1ScaleFactor));
		}

		if (RenderData.LeftSectorFloor > RenderData.RightSectorFloor) {
			RenderData.bDrawLowerSection = true;
			RenderData.LowerHeightStep = -(RenderData.LeftSectorFloor * RenderData.Steps);
			RenderData.iLowerHeight =
			    round(m_HalfScreenHeight - (RenderData.LeftSectorFloor * RenderData.V1ScaleFactor));
		}
	}

	renderSegment(RenderData);
}

void ViewRenderer::ceilingFloorUpdate(SegmentRenderData &RenderData) {
	if (!RenderData.pSeg->pLeftSector) {
		RenderData.UpdateFloor = true;
		RenderData.UpdateCeiling = true;
		return;
	}

	if (RenderData.LeftSectorCeiling != RenderData.RightSectorCeiling) {
		RenderData.UpdateCeiling = true;
	} else {
		RenderData.UpdateCeiling = false;
	}

	if (RenderData.LeftSectorFloor != RenderData.RightSectorFloor) {
		RenderData.UpdateFloor = true;
	} else {
		RenderData.UpdateFloor = false;
	}

	if (RenderData.pSeg->pLeftSector->CeilingHeight <= RenderData.pSeg->pRightSector->FloorHeight ||
	    RenderData.pSeg->pLeftSector->FloorHeight >= RenderData.pSeg->pRightSector->CeilingHeight) {
		// closed door
		RenderData.UpdateCeiling = RenderData.UpdateFloor = true;
	}

	if (RenderData.pSeg->pRightSector->CeilingHeight <= m_pPlayer->getZPosition()) {
		// below view plane
		RenderData.UpdateCeiling = false;
	}

	if (RenderData.pSeg->pRightSector->FloorHeight >= m_pPlayer->getZPosition()) {
		// above view plane
		RenderData.UpdateFloor = false;
	}
}

float ViewRenderer::getScaleFactor(int VXScreen, Angle SegToNormalAngle, float DistanceToNormal) {
	static float MAX_SCALEFACTOR = 64.0f;
	static float MIN_SCALEFACTOR = 0.00390625f;

	Angle Angle90(90);

	Angle ScreenXAngle = m_ScreenXToAngle[VXScreen];
	Angle SkewAngle = m_ScreenXToAngle[VXScreen] + m_pPlayer->getAngle() - SegToNormalAngle;

	float ScreenXAngleCos = ScreenXAngle.getCosValue();
	float SkewAngleCos = SkewAngle.getCosValue();
	float ScaleFactor = (m_iDistancePlayerToScreen * SkewAngleCos) / (DistanceToNormal * ScreenXAngleCos);

	if (ScaleFactor > MAX_SCALEFACTOR) {
		ScaleFactor = MAX_SCALEFACTOR;
	} else if (MIN_SCALEFACTOR > ScaleFactor) {
		ScaleFactor = MIN_SCALEFACTOR;
	}

	return ScaleFactor;
}

void ViewRenderer::calculateCeilingFloorHeight(Seg &seg, int &VXScreen, float &DistanceToV, float &CeilingVOnScreen, float &FloorVOnScreen) {
	float Ceiling = seg.pRightSector->CeilingHeight - m_pPlayer->getZPosition();
	float Floor = seg.pRightSector->FloorHeight - m_pPlayer->getZPosition();

	Angle VScreenAngle = m_ScreenXToAngle[VXScreen];

	float DistanceToVScreen = m_iDistancePlayerToScreen / VScreenAngle.getCosValue();

	CeilingVOnScreen = (abs(Ceiling) * DistanceToVScreen) / DistanceToV;
	FloorVOnScreen = (abs(Floor) * DistanceToVScreen) / DistanceToV;

	if (Ceiling > 0) {
		CeilingVOnScreen = m_HalfScreenHeight - CeilingVOnScreen;
	} else {
		CeilingVOnScreen += m_HalfScreenHeight;
	}

	if (Floor > 0) {
		FloorVOnScreen = m_HalfScreenHeight - FloorVOnScreen;
	} else {
		FloorVOnScreen += m_HalfScreenHeight;
	}
}

void ViewRenderer::partialSeg(Seg &seg, Angle &V1Angle, Angle &V2Angle, float &DistanceToV, bool IsLeftSide) {
	float SideC = sqrt(pow(seg.pStartVertex->XPosition - seg.pEndVertex->XPosition, 2) +
	                   pow(seg.pStartVertex->YPosition - seg.pEndVertex->YPosition, 2));
	Angle V1toV2Span = V1Angle - V2Angle;
	float SINEAngleB = DistanceToV * V1toV2Span.getSinValue() / SideC;
	Angle AngleB(asinf(SINEAngleB) * 180.0 / M_PI);
	Angle AngleA(180 - V1toV2Span.getValue() - AngleB.getValue());

	Angle AngleVToFOV;
	if (IsLeftSide) {
		AngleVToFOV = V1Angle - (m_pPlayer->getAngle() + 45);
	} else {
		AngleVToFOV = (m_pPlayer->getAngle() - 45) - V2Angle;
	}

	Angle NewAngleB(180 - AngleVToFOV.getValue() - AngleA.getValue());
	DistanceToV = DistanceToV * AngleA.getSinValue() / NewAngleB.getSinValue();
}

void ViewRenderer::renderSegment(SegmentRenderData &RenderData) {
	int iXCurrent = RenderData.V1XScreen;

	while (iXCurrent <= RenderData.V2XScreen) {
		int CurrentCeilingEnd = RenderData.CeilingEnd;
		int CurrentFloorStart = RenderData.FloorStart;

		if (!validateRange(RenderData, iXCurrent, CurrentCeilingEnd, CurrentFloorStart)) {
			continue;
		}

		if (RenderData.pSeg->pLeftSector) {
			drawUpperSection(RenderData, iXCurrent, CurrentCeilingEnd);
			drawLowerSection(RenderData, iXCurrent, CurrentFloorStart);
		} else {
			drawMiddleSection(RenderData, iXCurrent, CurrentCeilingEnd, CurrentFloorStart);
		}

		RenderData.CeilingEnd += RenderData.CeilingStep;
		RenderData.FloorStart += RenderData.FloorStep;
		++iXCurrent;
	}
}

void ViewRenderer::drawMiddleSection(ViewRenderer::SegmentRenderData &RenderData, int iXCurrent, int CurrentCeilingEnd, int CurrentFloorStart) {
	uint8_t color = getSectionColor(RenderData.pSeg->pLinedef->pRightSidedef->MiddleTexture);
	drawVerticalLine(iXCurrent, CurrentCeilingEnd, CurrentFloorStart, color);
	m_CeilingClipHeight[iXCurrent] = m_iRenderYSize;
	m_FloorClipHeight[iXCurrent] = -1;
}

void ViewRenderer::drawLowerSection(ViewRenderer::SegmentRenderData &RenderData, int iXCurrent, int CurrentFloorStart) {
	if (RenderData.bDrawLowerSection) {
		int iLowerHeight = RenderData.iLowerHeight;
		RenderData.iLowerHeight += RenderData.LowerHeightStep;

		if (iLowerHeight <= m_CeilingClipHeight[iXCurrent]) {
			iLowerHeight = m_CeilingClipHeight[iXCurrent] + 1;
		}

		if (iLowerHeight <= CurrentFloorStart) {
			uint8_t color = getSectionColor(RenderData.pSeg->pLinedef->pRightSidedef->LowerTexture);
			drawVerticalLine(iXCurrent, iLowerHeight, CurrentFloorStart, color);
			m_FloorClipHeight[iXCurrent] = iLowerHeight;
		} else
			m_FloorClipHeight[iXCurrent] = CurrentFloorStart + 1;
	} else if (RenderData.UpdateFloor) {
		m_FloorClipHeight[iXCurrent] = CurrentFloorStart + 1;
	}
}

void ViewRenderer::drawUpperSection(ViewRenderer::SegmentRenderData &RenderData, int iXCurrent, int CurrentCeilingEnd) {
	if (RenderData.bDrawUpperSection) {
		int iUpperHeight = RenderData.iUpperHeight;
		RenderData.iUpperHeight += RenderData.UpperHeightStep;

		if (iUpperHeight >= m_FloorClipHeight[iXCurrent]) {
			iUpperHeight = m_FloorClipHeight[iXCurrent] - 1;
		}

		if (iUpperHeight >= CurrentCeilingEnd) {
			uint8_t color = getSectionColor(RenderData.pSeg->pLinedef->pRightSidedef->UpperTexture);
			drawVerticalLine(iXCurrent, CurrentCeilingEnd, iUpperHeight, color);
			m_CeilingClipHeight[iXCurrent] = iUpperHeight;
		} else {
			m_CeilingClipHeight[iXCurrent] = CurrentCeilingEnd - 1;
		}
	} else if (RenderData.UpdateCeiling) {
		m_CeilingClipHeight[iXCurrent] = CurrentCeilingEnd - 1;
	}
}

bool ViewRenderer::validateRange(ViewRenderer::SegmentRenderData &RenderData, int &iXCurrent, int &CurrentCeilingEnd, int &CurrentFloorStart) {
	if (CurrentCeilingEnd < m_CeilingClipHeight[iXCurrent] + 1) {
		CurrentCeilingEnd = m_CeilingClipHeight[iXCurrent] + 1;
	}

	if (CurrentFloorStart >= m_FloorClipHeight[iXCurrent]) {
		CurrentFloorStart = m_FloorClipHeight[iXCurrent] - 1;
	}

	if (CurrentCeilingEnd > CurrentFloorStart) {
		RenderData.CeilingEnd += RenderData.CeilingStep;
		RenderData.FloorStart += RenderData.FloorStep;
		++iXCurrent;
		return false;
	}
	return true;
}

void ViewRenderer::render3DView() { m_pMap->render3DView(); }

int ViewRenderer::angleToScreen(Angle angle) {
	int iX = 0;

	if (angle > 90) {
		angle -= 90;
		iX = m_iDistancePlayerToScreen - round(angle.getTanValue() * m_HalfScreenWidth);
	} else {
		angle = 90 - angle.getValue();
		iX = round(angle.getTanValue() * m_HalfScreenWidth);
		iX += m_iDistancePlayerToScreen;
	}

	return iX;
}

uint8_t ViewRenderer::getSectionColor(const std::string &TextureName) {
	uint8_t color;
	if (m_WallColor.count(TextureName)) {
		color = m_WallColor[TextureName];
	} else {
		color = rand() % 256;
		m_WallColor[TextureName] = color;
	};

	return color;
}

void ViewRenderer::drawVerticalLine(int iX, int iStartY, int iEndY, uint8_t color) {
	while (iStartY < iEndY) {
		m_pScreenBuffer[m_iBufferPitch * iStartY + iX] = color;
		++iStartY;
	}
}
