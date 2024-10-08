#include "Map.h"
#include <climits>
#include <cstring>
#include <iostream>
#include <stdlib.h>

Map::Map(ViewRenderer *pViewRenderer, const std::string sName, Player *pPlayer, Things *pThings)
    : m_sName(sName), m_XMin(INT_MAX), m_XMax(INT_MIN), m_YMin(INT_MAX), m_YMax(INT_MIN), m_iLumpIndex(-1), m_pPlayer(pPlayer), m_pThings(pThings), m_pViewRenderer(pViewRenderer) {
	m_pSectors = new std::vector<WADSector>();
	m_pSidedefs = new std::vector<WADSidedef>();
	m_pLinedefs = new std::vector<WADLinedef>();
	m_pSegs = new std::vector<WADSeg>();
}

Map::~Map() {}

void Map::init() {
	buildSectors();
	buildSidedefs();
	buildLinedef();
	buildSeg();
}

void Map::buildSectors() {
	WADSector wadsector;
	Sector    sector;
	for (size_t i = 0; i < m_pSectors->size(); ++i) {
		wadsector = m_pSectors->at(i);
		sector.FloorHeight = wadsector.FloorHeight;
		sector.CeilingHeight = wadsector.CeilingHeight;
		std::strncpy(sector.FloorTexture, wadsector.FloorTexture, 8);
		sector.FloorTexture[8] = '\0';
		std::strncpy(sector.CeilingTexture, wadsector.CeilingTexture, 8);
		sector.CeilingTexture[8] = '\0';
		sector.Lightlevel = wadsector.Lightlevel;
		sector.Type = wadsector.Type;
		sector.Tag = wadsector.Tag;
		m_Sectors.push_back(sector);
	}
	delete m_pSectors;
	m_pSectors = nullptr;
}

void Map::buildSidedefs() {
	WADSidedef wadsidedef;
	Sidedef    sidedef;
	for (size_t i = 0; i < m_pSidedefs->size(); ++i) {
		wadsidedef = m_pSidedefs->at(i);
		sidedef.XOffset = wadsidedef.XOffset;
		sidedef.YOffset = wadsidedef.YOffset;
		std::strncpy(sidedef.UpperTexture, wadsidedef.UpperTexture, 8);
		sidedef.UpperTexture[8] = '\0';
		std::strncpy(sidedef.LowerTexture, wadsidedef.LowerTexture, 8);
		sidedef.LowerTexture[8] = '\0';
		std::strncpy(sidedef.MiddleTexture, wadsidedef.MiddleTexture, 8);
		sidedef.MiddleTexture[8] = '\0';
		sidedef.pSector = &m_Sectors[wadsidedef.SectorID];
		m_Sidedefs.push_back(sidedef);
	}
	delete m_pSidedefs;
	m_pSidedefs = nullptr;
}

void Map::buildLinedef() {
	WADLinedef wadlinedef;
	Linedef    linedef;

	for (size_t i = 0; i < m_pLinedefs->size(); ++i) {
		wadlinedef = m_pLinedefs->at(i);

		linedef.pStartVertex = &m_Vertexes[wadlinedef.StartVertexID];
		linedef.pEndVertex = &m_Vertexes[wadlinedef.EndVertexID];
		linedef.Flags = wadlinedef.Flags;
		linedef.LineType = wadlinedef.LineType;
		linedef.SectorTag = wadlinedef.SectorTag;

		if (wadlinedef.RightSidedef == 0xFFFF) {
			linedef.pRightSidedef = nullptr;
		} else {
			linedef.pRightSidedef = &m_Sidedefs[wadlinedef.RightSidedef];
		}

		if (wadlinedef.LeftSidedef == 0xFFFF) {
			linedef.pLeftSidedef = nullptr;
		} else {
			linedef.pLeftSidedef = &m_Sidedefs[wadlinedef.LeftSidedef];
		}

		m_Linedefs.push_back(linedef);
	}

	delete m_pLinedefs;
	m_pLinedefs = nullptr;
}

void Map::buildSeg() {
	WADSeg wadseg;
	Seg    seg;

	for (int i = 0; i < m_pSegs->size(); ++i) {
		wadseg = m_pSegs->at(i);

		seg.pStartVertex = &m_Vertexes[wadseg.StartVertexID];
		seg.pEndVertex = &m_Vertexes[wadseg.EndVertexID];
		// 8.38190317e-8 is to conver from Binary angles (BAMS) to float
		seg.SlopeAngle = ((float) (wadseg.SlopeAngle << 16) * 8.38190317e-8);
		seg.pLinedef = &m_Linedefs[wadseg.LinedefID];
		seg.Direction = wadseg.Direction;
		seg.Offset = (float) (wadseg.Offset << 16) / (float) (1 << 16);

		Sidedef *pRightSidedef;
		Sidedef *pLeftSidedef;

		if (seg.Direction) {
			pRightSidedef = seg.pLinedef->pLeftSidedef;
			pLeftSidedef = seg.pLinedef->pRightSidedef;
		} else {
			pRightSidedef = seg.pLinedef->pRightSidedef;
			pLeftSidedef = seg.pLinedef->pLeftSidedef;
		}

		if (pRightSidedef) {
			seg.pRightSector = pRightSidedef->pSector;
		} else {
			seg.pRightSector = nullptr;
		}

		if (pLeftSidedef) {
			seg.pLeftSector = pLeftSidedef->pSector;
		} else {
			seg.pLeftSector = nullptr;
		}

		m_Segs.push_back(seg);
	}

	delete m_pSegs;
	m_pSegs = nullptr;
}

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

int Map::getXMin() const { return m_XMin; }

int Map::getXMax() const { return m_XMax; }

int Map::getYMin() const { return m_YMin; }

int Map::getYMax() const { return m_YMax; }

void Map::addSidedef(WADSidedef &sidedef) { m_pSidedefs->push_back(sidedef); }

void Map::addSector(WADSector &sector) { m_pSectors->push_back(sector); }

void Map::addLinedef(WADLinedef &l) { m_pLinedefs->push_back(l); }

void Map::addNode(Node &node) { m_Nodes.push_back(node); }

void Map::addSubsector(Subsector &subsector) { m_Subsector.push_back(subsector); }

void Map::addSeg(WADSeg &seg) { m_pSegs->push_back(seg); }

std::string Map::getName() const { return m_sName; }

void Map::render3DView() { renderBSPNodes(); }

void Map::renderBSPNodes() { renderBSPNodes(m_Nodes.size() - 1); }

void Map::renderBSPNodes(int iNodeID) {
	// Masking all the bits exipt the last one
	// to check if this is a subsector
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
	Subsector &subsector = m_Subsector[iSubsectorID];

	for (size_t i = 0; i < subsector.SegCount; i++) {
		Seg  &seg = m_Segs[subsector.FirstSegID + i];
		Angle V1Angle, V2Angle, V1AngleFromPlayer, V2AngleFromPlayer;
		if (m_pPlayer->clipVertexesInFOV(*(seg.pStartVertex), *(seg.pEndVertex), V1Angle, V2Angle, V1AngleFromPlayer, V2AngleFromPlayer)) {
			m_pViewRenderer->addWallInFOV(seg, V1Angle, V2Angle, V1AngleFromPlayer, V2AngleFromPlayer);
		}
	}
}

bool Map::isPointOnLeftSide(int XPosition, int YPosition, int iNodeID) {
	int dx = XPosition - m_Nodes[iNodeID].XPartition;
	int dy = YPosition - m_Nodes[iNodeID].YPartition;

	return (((dx * m_Nodes[iNodeID].ChangeYPartition) - (dy * m_Nodes[iNodeID].ChangeXPartition)) <= 0);
}

void Map::setLumpIndex(int iIndex) { m_iLumpIndex = iIndex; }

int Map::getLumpIndex() const { return m_iLumpIndex; }

Things *Map::getThings() const { return m_pThings; }

int Map::getPlayerSubSectorHeight() {
	int iSubsectorID = m_Nodes.size() - 1;
	while (!(iSubsectorID & SUBSECTORIDENTIFIER)) {
		bool isOnLeft = isPointOnLeftSide(m_pPlayer->getXPosition(), m_pPlayer->getYPosition(), iSubsectorID);

		if (isOnLeft) {
			iSubsectorID = m_Nodes[iSubsectorID].LeftChildID;
		} else {
			iSubsectorID = m_Nodes[iSubsectorID].RightChildID;
		}
	}
	Subsector &subsector = m_Subsector[iSubsectorID & (~SUBSECTORIDENTIFIER)];
	Seg       &seg = m_Segs[subsector.FirstSegID];
	return seg.pRightSector->FloorHeight;
}