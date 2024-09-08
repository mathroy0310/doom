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
	void addWallInFOV(Seg &seg, Angle V1Angle, Angle V2Angle, Angle V1AngleFromPlayer, Angle V2AngleFromPlayer);
	void initFrame();
	void setDrawColor(Uint8 r, Uint8 g, Uint8 b);
	void drawRect(int x, int y, int w, int h);
	void drawLine(int x1, int y1, int x2, int y2);

  protected:
	struct SolidSegmentRange {
		int XStart;
		int XEnd;
	};

	void renderAutoMap();
	void render3DView();

	void clipSolidWalls(Seg &seg, int V1XScreen, int V2XScreen, Angle V1Angle, Angle V2Angle);
	void storeWallRange(Seg &seg, int V1XScreen, int V2XScreen, Angle V1Angle, Angle V2Angle);

	void calculateWallHeight(Seg &seg, int V1XScreen, int V2XScreen, Angle V1Angle, Angle V2Angle);
	void calculateWallHeightSimple(Seg &seg, int V1XScreen, int V2XScreen, Angle V1Angle, Angle V2Angle);
	void calculateCeilingFloorHeight(Seg &seg, int &VXScreen, float &DistanceToV, float &CeilingVOnScreen, float &FloorVOnScreen);
	void partialSeg(Seg &seg, Angle &V1Angle, Angle &V2Angle, float &DistanceToV1, bool IsLeftSide);
	void renderSolidWall(Seg &seg, int XStart, int XStop);

	float getScaleFactor(int VXScreen, Angle NormalAngle, float NormalDistance);

	int angleToScreen(Angle angle);
	int remapXToScreen(int XMapPosition);
	int remapYToScreen(int YMapPosition);

	SDL_Color getWallColor(const std::string textureName);

  private:
	int m_iRenderXSize;
	int m_iRenderYSize;
	int m_iAutoMapScaleFactor;
	int m_iDistancePlayerToScreen;
	int m_HalfScreenWidth;
	int m_HalfScreenHeight;

	bool m_UseClassicDoomScreenToAngle;

	Map          *m_pMap;
	Player       *m_pPlayer;
	SDL_Renderer *m_pRenderer;

	std::list<SolidSegmentRange>     m_SolidWallRanges;
	std::map<std::string, SDL_Color> m_WallColor;
	std::map<int, Angle>             m_ScreenXToAngle;
};