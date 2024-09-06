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
	void renderAutoMap();
	void renderAutoMapPlayer();
	void renderAutoMapWalls();

	std::string getName() const;
	void        setLumpIndex(int iLumpIndex);
	int         getLumpIndex() const;

  protected:
	int                  remapXToScreen(int XMapPosition);
	int                  remapYToScreen(int YMapPosition);
	std::string          m_sName;
	std::vector<Vertex>  m_Vertexes;
	std::vector<Linedef> m_Linedef;
	std::vector<Thing>   m_Things;

	int           m_XMin;
	int           m_XMax;
	int           m_YMin;
	int           m_YMax;
	int           m_iRenderXSize;
	int           m_iRenderYSize;
	int           m_iAutoMapScaleFactor;
	int           m_iLumpIndex;

	SDL_Renderer *m_pRenderer;
	Player       *m_pPlayer;
};