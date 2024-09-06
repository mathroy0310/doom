#include "Map.h"

#include <climits>

Map::Map(std::string sName, Player *pPlayer)
    : m_sName(sName), m_XMin(INT_MAX), m_XMax(INT_MIN), m_YMin(INT_MAX), m_YMax(INT_MIN), m_iAutoMapScaleFactor(15), m_iLumpIndex(-1), m_pPlayer(pPlayer) {}

Map::~Map() {}

void Map::addVertex(Vertex &v) {
	m_Vertexes.push_back(v);

	if (m_XMin > v.XPosition) {
		m_XMin = v.XPosition;
	} else if (m_XMax < v.XPosition) {
		m_XMax = v.XPosition;
	}

	if (m_YMin > v.YPosition) {
		m_YMin = v.YPosition;
	} else if (m_YMax < v.YPosition) {
		m_YMax = v.YPosition;
	}
}

void Map::addLinedef(Linedef &l) { m_Linedef.push_back(l); }

void Map::addThing(Thing &thing) {
	if (thing.Type == m_pPlayer->getID()) {
		m_pPlayer->setXPosition(thing.XPosition);
		m_pPlayer->setYPosition(thing.YPosition);
		m_pPlayer->setAngle(thing.Angle);
	}

	m_Things.push_back(thing);
}

std::string Map::getName() const { return m_sName; }

void Map::renderAutoMap(SDL_Renderer *pRenderer) {
	int iXShift = -m_XMin;
	int iYShift = -m_YMin;

	renderAutoMapWalls(pRenderer, iXShift, iYShift);
	renderAutoMapPlayer(pRenderer, iXShift, iYShift);
}

void Map::renderAutoMapPlayer(SDL_Renderer *pRenderer, int iXShift, int iYShift) {
	int iRenderXSize;
	int iRenderYSize;

	SDL_RenderGetLogicalSize(pRenderer, &iRenderXSize, &iRenderYSize);

	--iRenderXSize;
	--iRenderYSize;

	SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, SDL_ALPHA_OPAQUE);

	std::pair<int, int> Direction[] = {
	    std::make_pair(-1, -1), std::make_pair(0, -1), std::make_pair(+1, -1),
	    std::make_pair(-1, 0),  std::make_pair(0, 0),  std::make_pair(+1, 0),
	    std::make_pair(-1, +1), std::make_pair(0, +1), std::make_pair(+1, +1)};

	for (int i = 0; i < 9; ++i) {
		SDL_RenderDrawPoint(
		    pRenderer, (m_pPlayer->getXPosition() + iXShift) / m_iAutoMapScaleFactor + Direction[i].first,
		    iRenderYSize - (m_pPlayer->getYPosition() + iYShift) / m_iAutoMapScaleFactor +
		        Direction[i].second);
	}
}

void Map::renderAutoMapWalls(SDL_Renderer *pRenderer, int iXShift, int iYShift) {
	int iRenderXSize;
	int iRenderYSize;

	SDL_RenderGetLogicalSize(pRenderer, &iRenderXSize, &iRenderYSize);

	--iRenderXSize;
	--iRenderYSize;

	SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

	for (Linedef &l : m_Linedef) {
		Vertex vStart = m_Vertexes[l.StartVertex];
		Vertex vEnd = m_Vertexes[l.EndVertex];

		SDL_RenderDrawLine(pRenderer, (vStart.XPosition + iXShift) / m_iAutoMapScaleFactor,
		                   iRenderYSize - (vStart.YPosition + iYShift) / m_iAutoMapScaleFactor, (vEnd.XPosition + iXShift) / m_iAutoMapScaleFactor, iRenderYSize - (vEnd.YPosition + iYShift) / m_iAutoMapScaleFactor);
	}
}

void Map::setLumpIndex(int iIndex) { m_iLumpIndex = iIndex; }

int Map::getLumpIndex() const { return m_iLumpIndex; }