#include "Map.h"

#include <climits>

Map::Map(std::string sName)
    : m_sName(sName), m_XMin(INT_MAX), m_XMax(INT_MIN), m_YMin(INT_MAX), m_YMax(INT_MIN), m_iAutoMapScaleFactor(15) {}

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

std::string Map::getName() { return m_sName; }

void Map::renderAutoMap(SDL_Renderer *pRenderer) {
	int iXShift = -m_XMin;
	int iYShift = -m_YMin;

	int iRenderXSize;
	int iRenderYSize;

	// Read what is the resolution we are using
	SDL_RenderGetLogicalSize(pRenderer, &iRenderXSize, &iRenderYSize);

	--iRenderXSize; // remember it is 0 to 319 and not to 320
	--iRenderYSize; // remember it is 0 to 199 and not to 200

	SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

	for (Linedef &l : m_Linedef) {
		Vertex vStart = m_Vertexes[l.StartVertex];
		Vertex vEnd = m_Vertexes[l.EndVertex];

		SDL_RenderDrawLine(pRenderer, (vStart.XPosition + iXShift) / m_iAutoMapScaleFactor,
		                   iRenderYSize - (vStart.YPosition + iYShift) / m_iAutoMapScaleFactor, (vEnd.XPosition + iXShift) / m_iAutoMapScaleFactor, iRenderYSize - (vEnd.YPosition + iYShift) / m_iAutoMapScaleFactor);
	}
}