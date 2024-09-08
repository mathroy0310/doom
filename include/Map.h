#pragma once

#include <string>
#include <vector>

#include <SDL.h>

#include "DataTypes.h"
#include "Player.h"

class Map {
  public:
	Map(ViewRenderer *pViewRenderer, std::string sName, Player *pPlayer, Things *pThings);
	~Map();

	std::string getName() const;
	void        addVertex(Vertex &v);
	void        addLinedef(WADLinedef &l);
	void        addNode(Node &node);
	void        addSubsector(Subsector &subsector);
	void        addSeg(WADSeg &seg);
	void        addSidedef(WADSidedef &sidedef);
	void        addSector(WADSector &sector);
	void        renderAutoMap();
	void        render3DView();
	void        setLumpIndex(int iIndex);
	void        init();

	int getXMin() const;
	int getXMax() const;
	int getYMin() const;
	int getYMax() const;
	int getLumpIndex() const;

	Things *getThings() const;

  protected:
	void buildSectors();
	void buildSidedefs();
	void buildLinedef();
	void buildSeg();
	void renderBSPNodes();
	void renderAutoMapNode(int iNodeID);
	void renderBSPNodes(int iNodeID);
	void renderSubsector(int iSubsectorID);

	bool isPointOnLeftSide(int XPosition, int YPosition, int iNodeID);

  private:
	std::string m_sName;

	std::vector<Vertex>    m_Vertexes;
	std::vector<Sector>    m_Sectors;
	std::vector<Sidedef>   m_Sidedefs;
	std::vector<Linedef>   m_Linedefs;
	std::vector<Seg>       m_Segs;
	std::vector<Subsector> m_Subsector;
	std::vector<Node>      m_Nodes;

	std::vector<WADSector>  *m_pSectors;
	std::vector<WADSidedef> *m_pSidedefs;
	std::vector<WADLinedef> *m_pLinedefs;
	std::vector<WADSeg>     *m_pSegs;

	int m_XMin;
	int m_XMax;
	int m_YMin;
	int m_YMax;
	int m_iLumpIndex;

	Player       *m_pPlayer;
	Things       *m_pThings;
	ViewRenderer *m_pViewRenderer;
};