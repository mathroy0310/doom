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
	--m_iRenderXSize;
	--m_iRenderYSize;
}

int ViewRenderer::remapXToScreen(int XMapPosition) {
	return (XMapPosition + (-m_pMap->getXMin())) / m_iAutoMapScaleFactor;
}

int ViewRenderer::remapYToScreen(int YMapPosition) {
	return m_iRenderYSize - (YMapPosition + (-m_pMap->getYMin())) / m_iAutoMapScaleFactor;
}

void ViewRenderer::render(bool isAutomap) {
	if (isAutomap) {
		renderAutoMap();
	} else {
		render3DView();
	}
	SDL_RenderPresent(m_pRenderer);
}

void ViewRenderer::initFrame() {
	setDrawColor(0, 0, 0);
	SDL_RenderClear(m_pRenderer);
}

void ViewRenderer::addWallInFOV(Seg &seg, Angle V1Angle, Angle V2Angle) {
	if (seg.pLeftSector == nullptr) {
		addSolidWall(seg, V1Angle, V2Angle);
	}
}

void ViewRenderer::addSolidWall(Seg &seg, Angle V1Angle, Angle V2Angle) {
	int V1XScreen = angleToScreen(V1Angle);
	int V2XScreen = angleToScreen(V2Angle);

	SDL_RenderDrawLine(m_pRenderer, V1XScreen, 0, V1XScreen, m_iRenderYSize);
	SDL_RenderDrawLine(m_pRenderer, V2XScreen, 0, V2XScreen, m_iRenderYSize);
}

void ViewRenderer::renderAutoMap() {
	m_pMap->renderAutoMap();
	m_pPlayer->renderAutoMap();
}

void ViewRenderer::render3DView()
{
    m_pMap->render3DView();
}

int ViewRenderer::angleToScreen(Angle angle) {
	Angle tmpAngle = angle;
	int   iX = 0;

	if (angle > 90) {
		angle -= 90;
		iX = 160 - round(tanf(angle.getValue() * M_PI / 180.f) * 160);
		iX += 160;
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

void ViewRenderer::drawLine(int X1, int Y1, int X2, int Y2)
{
    SDL_RenderDrawLine(m_pRenderer,
        remapXToScreen(X1),
        remapYToScreen(Y1),
        remapXToScreen(X2),
        remapYToScreen(Y2));
}