#include "ViewRenderer.h"

#include <iostream>
#include <math.h>

#include "ClassicDefs.h"
#include "Map.h"
#include "Player.h"

ViewRenderer::ViewRenderer(SDL_Renderer *pRenderer)
    : m_pRenderer(pRenderer), m_iAutoMapScaleFactor(15), m_UseClassicDoomScreenToAngle(false) {}

ViewRenderer::~ViewRenderer() {}

void ViewRenderer::init(Map *pMap, Player *pPlayer) {
	m_pMap = pMap;
	m_pPlayer = pPlayer;

	SDL_RenderGetLogicalSize(m_pRenderer, &m_iRenderXSize, &m_iRenderYSize);

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
}

int ViewRenderer::remapXToScreen(int XMapPosition) {
	return (XMapPosition + (-m_pMap->getXMin())) / m_iAutoMapScaleFactor;
}

int ViewRenderer::remapYToScreen(int YMapPosition) {
	return m_iRenderYSize - (YMapPosition + (-m_pMap->getYMin())) / m_iAutoMapScaleFactor;
}

void ViewRenderer::render(bool isRenderAutoMap) {
	if (isRenderAutoMap) {
		renderAutoMap();
	} else {
		render3DView();
	}
	SDL_RenderPresent(m_pRenderer);
}

void ViewRenderer::initFrame() {
	setDrawColor(0, 0, 0);
	SDL_RenderClear(m_pRenderer);

	m_SolidWallRanges.clear();

	SolidSegmentRange WallLeftSide;
	SolidSegmentRange WallRightSide;

	WallLeftSide.XStart = INT_MIN;
	WallLeftSide.XEnd = -1;
	m_SolidWallRanges.push_back(WallLeftSide);

	WallRightSide.XStart = m_iRenderXSize;
	WallRightSide.XEnd = INT_MAX;
	m_SolidWallRanges.push_back(WallRightSide);
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

void ViewRenderer::storeWallRange(Seg &seg, int V1XScreen, int V2XScreen, Angle V1Angle, Angle V2Angle) {
	calculateWallHeight(seg, V1XScreen, V2XScreen, V1Angle, V2Angle);
	calculateWallHeightSimple(seg, V1XScreen, V2XScreen, V1Angle, V2Angle);
}

void ViewRenderer::calculateWallHeight(Seg &seg, int V1XScreen, int V2XScreen, Angle V1Angle, Angle V2Angle) {
	// Calculate the distance to the first edge of the wall
	Angle Angle90(90);
	Angle SegToNormalAngle = seg.SlopeAngle + Angle90;
	// Angle NomalToV1Angle = abs(SegToNormalAngle.GetSignedValue() - V1Angle.GetSignedValue());
	Angle NomalToV1Angle = SegToNormalAngle.getValue() - V1Angle.getValue();

	// Normal angle is 90 degree to wall
	Angle SegToPlayerAngle = Angle90 - NomalToV1Angle;

	float DistanceToV1 = m_pPlayer->distanceToPoint(*seg.pStartVertex);
	float DistanceToNormal = SegToPlayerAngle.getSinValue() * DistanceToV1;

	float V1ScaleFactor = getScaleFactor(V1XScreen, SegToNormalAngle, DistanceToNormal);
	float V2ScaleFactor = getScaleFactor(V2XScreen, SegToNormalAngle, DistanceToNormal);

	float Steps = (V2ScaleFactor - V1ScaleFactor) / (V2XScreen - V1XScreen);

	float Ceiling = seg.pRightSector->CeilingHeight - m_pPlayer->getZPosition();
	float Floor = seg.pRightSector->FloorHeight - m_pPlayer->getZPosition();

	float CeilingStep = -(Ceiling * Steps);
	float CeilingEnd = m_HalfScreenHeight - (Ceiling * V1ScaleFactor);

	float     FloorStep = -(Floor * Steps);
	float     FloorStart = m_HalfScreenHeight - (Floor * V1ScaleFactor);

	SDL_Color color = getWallColor(seg.pLinedef->pRightSidedef->MiddleTexture);
	setDrawColor(color.r, color.g, color.b);

	int iXCurrent = V1XScreen;
	while (iXCurrent <= V2XScreen) {
		SDL_RenderDrawLine(m_pRenderer, iXCurrent, CeilingEnd, iXCurrent, FloorStart);
		++iXCurrent;

		CeilingEnd += CeilingStep;
		FloorStart += FloorStep;
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

void ViewRenderer::calculateWallHeightSimple(Seg &seg, int V1XScreen, int V2XScreen, Angle V1Angle, Angle V2Angle) {
	// We have V1 and V2, do calculations for V1 and V2 sepratly then interpolate values in between
	float DistanceToV1 = m_pPlayer->distanceToPoint(*seg.pStartVertex);
	float DistanceToV2 = m_pPlayer->distanceToPoint(*seg.pEndVertex);

	// Special Case partial seg on the left
	if (V1XScreen <= 0) {
		partialSeg(seg, V1Angle, V2Angle, DistanceToV1, true);
	}

	// Special Case partial seg on the right
	if (V2XScreen >= 319) {
		partialSeg(seg, V1Angle, V2Angle, DistanceToV2, false);
	}

	float CeilingV1OnScreen;
	float FloorV1OnScreen;
	float CeilingV2OnScreen;
	float FloorV2OnScreen;

	calculateCeilingFloorHeight(seg, V1XScreen, DistanceToV1, CeilingV1OnScreen, FloorV1OnScreen);
	calculateCeilingFloorHeight(seg, V2XScreen, DistanceToV2, CeilingV2OnScreen, FloorV2OnScreen);

	SDL_Color color = { 255,255,255 };
	// SDL_Color color = getWallColor(seg.pLinedef->pRightSidedef->MiddleTexture);
	setDrawColor(color.r, color.g, color.b);

	SDL_RenderDrawLine(m_pRenderer, V1XScreen, CeilingV1OnScreen, V1XScreen, FloorV1OnScreen);
	SDL_RenderDrawLine(m_pRenderer, V2XScreen, CeilingV2OnScreen, V2XScreen, FloorV2OnScreen);
	SDL_RenderDrawLine(m_pRenderer, V1XScreen, CeilingV1OnScreen, V2XScreen, CeilingV2OnScreen);
	SDL_RenderDrawLine(m_pRenderer, V1XScreen, FloorV1OnScreen, V2XScreen, FloorV2OnScreen);
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

void ViewRenderer::renderSolidWall(Seg &seg, int XStart, int XStop) {
	int iXCurrent = XStart;

	SDL_Color color = getWallColor(seg.pLinedef->pRightSidedef->MiddleTexture);
	setDrawColor(color.r, color.g, color.b);
	while (iXCurrent <= XStop) {
		SDL_RenderDrawLine(m_pRenderer, iXCurrent, 10, iXCurrent, 20);
		++iXCurrent;
	}
}

void ViewRenderer::renderAutoMap() {
	m_pMap->renderAutoMap();
	m_pPlayer->renderAutoMap();
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

void ViewRenderer::setDrawColor(Uint8 r, Uint8 g, Uint8 b) {
	SDL_SetRenderDrawColor(m_pRenderer, r, g, b, SDL_ALPHA_OPAQUE);
}

void ViewRenderer::drawRect(int X1, int Y1, int X2, int Y2) {
	SDL_Rect Rect = {remapXToScreen(X1), remapYToScreen(Y1), remapXToScreen(X2) - remapXToScreen(X1) + 1, remapYToScreen(Y2) - remapYToScreen(Y1) + 1};

	SDL_RenderDrawRect(m_pRenderer, &Rect);
}

void ViewRenderer::drawLine(int X1, int Y1, int X2, int Y2) {
	SDL_RenderDrawLine(m_pRenderer, remapXToScreen(X1), remapYToScreen(Y1), remapXToScreen(X2), remapYToScreen(Y2));
}

SDL_Color ViewRenderer::getWallColor(const std::string textureName) {
	if (m_WallColor.count(textureName)) {
		return m_WallColor[textureName];
	} else {
		SDL_Color color{static_cast<Uint8>(rand() % 255), static_cast<Uint8>(rand() % 255), static_cast<Uint8>(rand() % 255)};
		m_WallColor[textureName] = color;
		return color;
	}
}