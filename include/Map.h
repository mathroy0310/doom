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

	void renderAutoMap();
	void renderAutoMapPlayer();
	void renderAutoMapWalls();
	void renderAutoMapNode();

	std::string getName() const;
	void        setLumpIndex(int iLumpIndex);
	int         getLumpIndex() const;

  protected:
	int                  remapXToScreen(int XMapPosition) const;
	int                  remapYToScreen(int YMapPosition) const;
	std::string          m_sName;
	std::vector<Vertex>  m_Vertexes;
	std::vector<Linedef> m_Linedef;
	std::vector<Thing>   m_Things;
	std::vector<Node>    m_Nodes;

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