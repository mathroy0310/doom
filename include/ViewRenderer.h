#pragma once

#include <SDL.h>

#include "Angle.h"
#include "DataTypes.h"

#include <climits>
#include <list>
#include <map>
#include <string>
#include <vector>

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

	struct SegmentRenderData {
		int   V1XScreen;
		int   V2XScreen;
		Angle V1Angle;
		Angle V2Angle;
		float DistanceToV1;
		float DistanceToNormal;
		float V1ScaleFactor;
		float V2ScaleFactor;
		float Steps;

		float RightSectorCeiling;
		float RightSectorFloor;
		float CeilingStep;
		float CeilingEnd;
		float FloorStep;
		float FloorStart;

		float LeftSectorCeiling;
		float LeftSectorFloor;

		bool bDrawUpperSection;
		bool bDrawLowerSection;

		float UpperHeightStep;
		float iUpperHeight;
		float LowerHeightStep;
		float iLowerHeight;

		bool UpdateFloor;
		bool UpdateCeiling;

		Seg *pSeg;
	};

	void renderAutoMap();
	void render3DView();

	void selectColor(Seg &seg, SDL_Color &color);
	void clipSolidWalls(Seg &seg, int V1XScreen, int V2XScreen, Angle V1Angle, Angle V2Angle);
	void clipPassWalls(Seg &seg, int V1XScreen, int V2XScreen, Angle V1Angle, Angle V2Angle);
	void storeWallRange(Seg &seg, int V1XScreen, int V2XScreen, Angle V1Angle, Angle V2Angle);

	void calculateWallHeight(Seg &seg, int V1XScreen, int V2XScreen, Angle V1Angle, Angle V2Angle);
	void calculateWallHeightSimple(Seg &seg, int V1XScreen, int V2XScreen, Angle V1Angle, Angle V2Angle);
	void ceilingFloorUpdate(ViewRenderer::SegmentRenderData &RenderData);
	void calculateCeilingFloorHeight(Seg &seg, int &VXScreen, float &DistanceToV, float &CeilingVOnScreen, float &FloorVOnScreen);
	void partialSeg(Seg &seg, Angle &V1Angle, Angle &V2Angle, float &DistanceToV1, bool IsLeftSide);

	void drawMiddleSection(ViewRenderer::SegmentRenderData &RenderData, int iXCurrent, int CurrentCeilingEnd, int CurrentFloorStart);
	void drawLowerSection(ViewRenderer::SegmentRenderData &RenderData, int iXCurrent, int CurrentFloorStart);
	void drawUpperSection(ViewRenderer::SegmentRenderData &RenderData, int iXCurrent, int CurrentCeilingEnd);
	void renderSegment(SegmentRenderData &RenderData);
	void renderSolidWall(Seg &seg, int XStart, int XStop);

	bool validateRange(ViewRenderer::SegmentRenderData &RenderData, int &iXCurrent, int &CurrentCeilingEnd, int &CurrentFloorStart);

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
	std::vector<int>                 m_FloorClipHeight;
	std::vector<int>                 m_CeilingClipHeight;
	std::map<std::string, SDL_Color> m_WallColor;
	std::map<int, Angle>             m_ScreenXToAngle;
};