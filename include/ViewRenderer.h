#pragma once

#include <SDL.h>

#include "Angle.h"
#include "DataTypes.h"

class Map;
class Player;

class ViewRenderer {
  public:
	ViewRenderer(SDL_Renderer *pRenderer);
	~ViewRenderer();

	void init(Map *pMap, Player *pPlayer);
	void render(bool isAutomap);
	void addWallInFOV(Seg &seg, Angle V1Angle, Angle V2Angle);
	void initFrame();
	void setDrawColor(Uint8 r, Uint8 g, Uint8 b);
	void drawRect(int x, int y, int w, int h);
	void drawLine(int x1, int y1, int x2, int y2);

  protected:
	void renderAutoMap();
	void render3DView();

	void addSolidWall(Seg &seg, Angle V1Angle, Angle V2Angle);

	int angleToScreen(Angle angle);
	int remapXToScreen(int XMapPosition);
	int remapYToScreen(int YMapPosition);

	int m_iRenderXSize;
	int m_iRenderYSize;
	int m_iAutoMapScaleFactor;

	Map          *m_pMap;
	Player       *m_pPlayer;
	SDL_Renderer *m_pRenderer;
};