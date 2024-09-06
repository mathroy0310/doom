#pragma once

#include <string>
#include <vector>

#include <SDL.h>

#include "DataTypes.h"
#include "Player.h"

class Map {
  public:
	Map(SDL_Renderer *pRenderer, std::string sName, Player *pPlayer);
	~Map();

	void addVertex(Vertex &v);
	void addLinedef(Linedef &l);
	void addThing(Thing &thing);
	void addNode(Node &node);
	void addSubsector(Subsector &subsector);
	void addSeg(Seg &seg);

	void renderAutoMap();

	std::string getName() const;
	void        setLumpIndex(int iLumpIndex);
	int         getLumpIndex() const;

  protected:
	void renderAutoMapPlayer();
	void renderAutoMapWalls();
	void renderAutoMapNode(int iNodeID);
	void renderBSPNodes();
	void renderBSPNodes(int iNodeID);
	void renderSubsector(int iSubsectorID);

	int remapXToScreen(int XMapPosition) const;
	int remapYToScreen(int YMapPosition) const;

	bool isPointOnLeftSide(int XPosition, int YPosition, int iNodeID);

  private:
	std::string            m_sName;
	std::vector<Vertex>    m_Vertexes;
	std::vector<Linedef>   m_Linedef;
	std::vector<Thing>     m_Things;
	std::vector<Node>      m_Nodes;
	std::vector<Subsector> m_Subsector;
	std::vector<Seg>       m_Segs;

	int m_XMin;
	int m_XMax;
	int m_YMin;
	int m_YMax;
	int m_iRenderXSize;
	int m_iRenderYSize;
	int m_iAutoMapScaleFactor;
	int m_iLumpIndex;

	SDL_Renderer *m_pRenderer;
	Player       *m_pPlayer;
};