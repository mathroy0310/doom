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
	ViewRenderer();
	~ViewRenderer();

	void init(Map *pMap, Player *pPlayer);
	void render(uint8_t *pScreenBuffer, int iBufferPitch);
	void addWallInFOV(Seg &seg, Angle V1Angle, Angle V2Angle, Angle V1AngleFromPlayer, Angle V2AngleFromPlayer);
	void initFrame();

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

	void render3DView();
	void drawVerticalLine(int iX, int iStartY, int iEndY, uint8_t color);

	void clipSolidWalls(Seg &seg, int V1XScreen, int V2XScreen, Angle V1Angle, Angle V2Angle);
	void clipPassWalls(Seg &seg, int V1XScreen, int V2XScreen, Angle V1Angle, Angle V2Angle);
	void storeWallRange(Seg &seg, int V1XScreen, int V2XScreen, Angle V1Angle, Angle V2Angle);

	void calculateWallHeight(Seg &seg, int V1XScreen, int V2XScreen, Angle V1Angle, Angle V2Angle);
	void ceilingFloorUpdate(ViewRenderer::SegmentRenderData &RenderData);
	void calculateCeilingFloorHeight(Seg &seg, int &VXScreen, float &DistanceToV, float &CeilingVOnScreen, float &FloorVOnScreen);
	void partialSeg(Seg &seg, Angle &V1Angle, Angle &V2Angle, float &DistanceToV1, bool IsLeftSide);
	void renderSegment(SegmentRenderData &RenderData);

	void drawMiddleSection(ViewRenderer::SegmentRenderData &RenderData, int iXCurrent, int CurrentCeilingEnd, int CurrentFloorStart);
	void drawLowerSection(ViewRenderer::SegmentRenderData &RenderData, int iXCurrent, int CurrentFloorStart);
	void drawUpperSection(ViewRenderer::SegmentRenderData &RenderData, int iXCurrent, int CurrentCeilingEnd);

	uint8_t getSectionColor(const std::string &TextureName);
	bool validateRange(ViewRenderer::SegmentRenderData &RenderData, int &iXCurrent, int &CurrentCeilingEnd, int &CurrentFloorStart);

	float getScaleFactor(int VXScreen, Angle NormalAngle, float NormalDistance);

	int angleToScreen(Angle angle);

  private:
	int m_iRenderXSize;
	int m_iRenderYSize;
	int m_iDistancePlayerToScreen;
	int m_HalfScreenWidth;
	int m_HalfScreenHeight;

	bool m_UseClassicDoomScreenToAngle;

	Map    *m_pMap;
	Player *m_pPlayer;

	std::list<SolidSegmentRange>   m_SolidWallRanges;
	std::vector<int>               m_FloorClipHeight;
	std::vector<int>               m_CeilingClipHeight;
	std::map<std::string, uint8_t> m_WallColor;
	std::map<int, Angle>           m_ScreenXToAngle;

	uint8_t *m_pScreenBuffer;
	int      m_iBufferPitch;
};