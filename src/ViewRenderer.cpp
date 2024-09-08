#include "ViewRenderer.h"

#include <iostream>
#include <math.h>

#include "Map.h"
#include "Player.h"

ViewRenderer::ViewRenderer(SDL_Renderer *pRenderer)
    : m_pRenderer(pRenderer), m_iAutoMapScaleFactor(15) {}

ViewRenderer::~ViewRenderer() {}

void ViewRenderer::init(Map *pMap, Player *pPlayer) {
	m_pMap = pMap;
	m_pPlayer = pPlayer;

	SDL_RenderGetLogicalSize(m_pRenderer, &m_iRenderXSize, &m_iRenderYSize);
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

void ViewRenderer::addWallInFOV(Seg &seg, Angle V1Angle, Angle V2Angle) {
	// Find Wall X Coordinates
	int V1XScreen = angleToScreen(V1Angle);
	int V2XScreen = angleToScreen(V2Angle);

	// Skip same pixel wall
	if (V1XScreen == V2XScreen) return;

	// Handle solid walls
	if (seg.pLeftSector == nullptr) {
		clipSolidWalls(seg, V1XScreen, V2XScreen);
	}
}

void ViewRenderer::clipSolidWalls(Seg &seg, int V1XScreen, int V2XScreen) {
	// Find clip window
	SolidSegmentRange CurrentWall = {V1XScreen, V2XScreen};

	std::list<SolidSegmentRange>::iterator FoundClipWall = m_SolidWallRanges.begin();
	while (FoundClipWall != m_SolidWallRanges.end() && FoundClipWall->XEnd < CurrentWall.XStart - 1) {
		++FoundClipWall;
	}

	if (CurrentWall.XStart < FoundClipWall->XStart) {
		if (CurrentWall.XEnd < FoundClipWall->XStart - 1) {
			// All of the wall is visible, so insert it
			storeWallRange(seg, CurrentWall.XStart, CurrentWall.XEnd);
			m_SolidWallRanges.insert(FoundClipWall, CurrentWall);
			return;
		}

		// The end is already included, just update start
		storeWallRange(seg, CurrentWall.XStart, FoundClipWall->XStart - 1);
		FoundClipWall->XStart = CurrentWall.XStart;
	}

	// This part is already occupied
	if (CurrentWall.XEnd <= FoundClipWall->XEnd) return;

	std::list<SolidSegmentRange>::iterator NextWall = FoundClipWall;

	while (CurrentWall.XEnd >= next(NextWall, 1)->XStart - 1) {
		// partialy clipped by other walls, store each fragment
		storeWallRange(seg, NextWall->XEnd + 1, next(NextWall, 1)->XStart - 1);
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

	storeWallRange(seg, NextWall->XEnd + 1, CurrentWall.XEnd);
	FoundClipWall->XEnd = CurrentWall.XEnd;

	if (NextWall != FoundClipWall) {
		FoundClipWall++;
		NextWall++;
		m_SolidWallRanges.erase(FoundClipWall, NextWall);
	}
}

void ViewRenderer::storeWallRange(Seg &seg, int V1XScreen, int V2XScreen) {
	// For now just we will not store the range, we will just draw it
	SolidSegmentData Wall = {seg, V1XScreen, V2XScreen};
	drawSolidWall(Wall);
}

void ViewRenderer::renderAutoMap() {
	m_pMap->renderAutoMap();
	m_pPlayer->renderAutoMap();
}

void ViewRenderer::render3DView() { m_pMap->render3DView(); }

void ViewRenderer::drawSolidWall(SolidSegmentData &visibleSeg) {
	SDL_Color color = getWallColor(visibleSeg.seg.pLinedef->pRightSidedef->MiddleTexture);
	SDL_Rect Rect = {visibleSeg.XStart, 0, visibleSeg.XEnd - visibleSeg.XStart + 1, m_iRenderYSize};
	setDrawColor(color.r, color.g, color.b);
	SDL_RenderFillRect(m_pRenderer, &Rect);
	// SDL_RenderPresent(m_pRenderer);
	// SDL_Delay(1000);
}

int ViewRenderer::angleToScreen(Angle angle) {
	Angle tmpAngle = angle;
	int   iX = 0;

	if (angle > 90) {
		angle -= 90;
		iX = 160 - round(tanf(angle.getValue() * M_PI / 180.f) * 160);
	} else {
		angle = 90 - angle.getValue();
		float f = tanf(angle.getValue());
		iX = round(tanf(angle.getValue() * M_PI / 180.f) * 160);
		iX += 160;
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