#pragma once

#include <SDL.h>

#include "Angle.h"
#include "DataTypes.h"

#include <climits>
#include <list>
#include <map>
#include <string>

class Map;
class Player;

class ViewRenderer {
  public:
	ViewRenderer(SDL_Renderer *pRenderer);
	~ViewRenderer();

	void init(Map *pMap, Player *pPlayer);
	void render(bool isRenderAutoMap);
	void addWallInFOV(Seg &seg, Angle V1Angle, Angle V2Angle);
	void initFrame();
	void setDrawColor(Uint8 r, Uint8 g, Uint8 b);
	void drawRect(int x, int y, int w, int h);
	void drawLine(int x1, int y1, int x2, int y2);

  protected:
	struct SolidSegmentRange {
		int XStart;
		int XEnd;
	};

	struct SolidSegmentData {
		Seg &seg;
		int  XStart;
		int  XEnd;
	};

	void renderAutoMap();
	void render3DView();
	void drawSolidWall(SolidSegmentData &visableSeg);

	void clipSolidWalls(Seg &seg, int V1XScreen, int V2XScreen);
	void storeWallRange(Seg &seg, int V1XScreen, int V2XScreen);

	int angleToScreen(Angle angle);
	int remapXToScreen(int XMapPosition);
	int remapYToScreen(int YMapPosition);

	SDL_Color getWallColor(const std::string textureName);

  private:
	int m_iRenderXSize;
	int m_iRenderYSize;
	int m_iAutoMapScaleFactor;

	Map          *m_pMap;
	Player       *m_pPlayer;
	SDL_Renderer *m_pRenderer;

	std::list<SolidSegmentRange>     m_SolidWallRanges;
	std::map<std::string, SDL_Color> m_WallColor;
};