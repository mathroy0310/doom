#include "Map.h"

#include <climits>

Map::Map(SDL_Renderer *pRenderer, std::string sName, Player *pPlayer)
    : m_pRenderer(pRenderer), m_sName(sName), m_XMin(INT_MAX), m_XMax(INT_MIN), m_YMin(INT_MAX), m_YMax(INT_MIN), m_iAutoMapScaleFactor(15), m_iLumpIndex(-1), m_pPlayer(pPlayer) {
	SDL_RenderGetLogicalSize(m_pRenderer, &m_iRenderXSize, &m_iRenderYSize);
	--m_iRenderXSize;
	--m_iRenderYSize;
}
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

void Map::addNode(Node &node) { m_Nodes.push_back(node); }

void Map::addSubsector(Subsector &subsector) { m_Subsector.push_back(subsector); }

void Map::addSeg(Seg &seg) { m_Segs.push_back(seg); }

std::string Map::getName() const { return m_sName; }

int Map::remapXToScreen(int XMapPosition) const {
	return (XMapPosition + (-m_XMin)) / m_iAutoMapScaleFactor;
}

int Map::remapYToScreen(int YMapPosition) const {
	return m_iRenderYSize - (YMapPosition + (-m_YMin)) / m_iAutoMapScaleFactor;
}

void Map::renderAutoMap() {
	renderAutoMapWalls();
	renderAutoMapPlayer();
	// renderAutoMapNode();
	renderBSPNodes();
}

void Map::renderBSPNodes() { renderBSPNodes(m_Nodes.size() - 1); }

void Map::renderBSPNodes(int iNodeID) {
	// masking all the bit except the last
	//  to check if its a subsector or not
	if (iNodeID & SUBSECTORIDENTIFIER) {
		renderSubsector(iNodeID & (~SUBSECTORIDENTIFIER));
		return;
	}

	bool isOnLeft = isPointOnLeftSide(m_pPlayer->getXPosition(), m_pPlayer->getYPosition(), iNodeID);

	if (isOnLeft) {
		renderBSPNodes(m_Nodes[iNodeID].LeftChildID);
		renderBSPNodes(m_Nodes[iNodeID].RightChildID);
	} else {
		renderBSPNodes(m_Nodes[iNodeID].RightChildID);
		renderBSPNodes(m_Nodes[iNodeID].LeftChildID);
	}
}

void Map::renderSubsector(int iSubsectorID) {
	Subsector subsector = m_Subsector[iSubsectorID];
	SDL_SetRenderDrawColor(m_pRenderer, 0, 255, 255, SDL_ALPHA_OPAQUE);

	for (int i = 0; i < subsector.SegCount; i++) {
		Seg seg = m_Segs[subsector.FirstSegID + i];
        Angle V1Angle, V2Angle;
        if (m_pPlayer->clipVertexesInFOV(m_Vertexes[seg.StartVertexID], m_Vertexes[seg.EndVertexID], V1Angle, V2Angle))
        {
            SDL_RenderDrawLine(m_pRenderer,
                remapXToScreen(m_Vertexes[seg.StartVertexID].XPosition),
                remapYToScreen(m_Vertexes[seg.StartVertexID].YPosition),
                remapXToScreen(m_Vertexes[seg.EndVertexID].XPosition),
                remapYToScreen(m_Vertexes[seg.EndVertexID].YPosition));
        }
	}
}

bool Map::isPointOnLeftSide(int XPosition, int YPosition, int iNodeID) {
	int dx = XPosition - m_Nodes[iNodeID].XPartition;
	int dy = YPosition - m_Nodes[iNodeID].YPartition;

	return (((dx * m_Nodes[iNodeID].ChangeYPartition) - (dy * m_Nodes[iNodeID].ChangeXPartition)) <= 0);
}

void Map::renderAutoMapPlayer() {
	SDL_SetRenderDrawColor(m_pRenderer, 255, 0, 0, SDL_ALPHA_OPAQUE);

	std::pair<int, int> Direction[] = {
	    std::make_pair(-1, -1), std::make_pair(0, -1), std::make_pair(+1, -1),
	    std::make_pair(-1, 0),  std::make_pair(0, 0),  std::make_pair(+1, 0),
	    std::make_pair(-1, +1), std::make_pair(0, +1), std::make_pair(+1, +1)};

	for (int i = 0; i < 9; ++i) {
		SDL_RenderDrawPoint(m_pRenderer, remapXToScreen(m_pPlayer->getXPosition()) + Direction[i].first,
		                    remapYToScreen(m_pPlayer->getYPosition()) + Direction[i].second);
	}
}

void Map::renderAutoMapWalls() {
	SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

	for (Linedef &l : m_Linedef) {
		Vertex vStart = m_Vertexes[l.StartVertex];
		Vertex vEnd = m_Vertexes[l.EndVertex];

		SDL_RenderDrawLine(m_pRenderer, remapXToScreen(vStart.XPosition), remapYToScreen(vStart.YPosition), remapXToScreen(vEnd.XPosition), remapYToScreen(vEnd.YPosition));
	}
}

void Map::renderAutoMapNode(int iNodeID) {
	Node node = m_Nodes[iNodeID];

	SDL_Rect RightRect = {remapXToScreen(node.RightBoxLeft), remapYToScreen(node.RightBoxTop), remapXToScreen(node.RightBoxRight) - remapXToScreen(node.RightBoxLeft) + 1, remapYToScreen(node.RightBoxBottom) - remapYToScreen(node.RightBoxTop) + 1};

	SDL_Rect LeftRect = {remapXToScreen(node.LeftBoxLeft), remapYToScreen(node.LeftBoxTop), remapXToScreen(node.LeftBoxRight) - remapXToScreen(node.LeftBoxLeft) + 1, remapYToScreen(node.LeftBoxBottom) - remapYToScreen(node.LeftBoxTop) + 1};

	SDL_SetRenderDrawColor(m_pRenderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(m_pRenderer, &RightRect);
	SDL_SetRenderDrawColor(m_pRenderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(m_pRenderer, &LeftRect);

	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(m_pRenderer, remapXToScreen(node.XPartition), remapYToScreen(node.YPartition), remapXToScreen(node.XPartition + node.ChangeXPartition), remapYToScreen(node.YPartition + node.ChangeYPartition));
}

void Map::setLumpIndex(int iIndex) { m_iLumpIndex = iIndex; }

int Map::getLumpIndex() const { return m_iLumpIndex; }