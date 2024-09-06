#pragma once

#include <string>
#include <vector>

#include <SDL.h>

#include "DataTypes.h"
#include "Player.h"

class Map {
  public:
	Map(std::string sName, Player *pPlayer);
	~Map();

	void addVertex(Vertex &v);
	void addLinedef(Linedef &l);
	void addThing(Thing &thing);
	void renderAutoMap(SDL_Renderer *pRenderer);
	void renderAutoMapPlayer(SDL_Renderer *pRenderer, int iXShift, int iYShift);
	void renderAutoMapWalls(SDL_Renderer *pRenderer, int iXShift, int iYShift);

	std::string getName() const;
	void        setLumpIndex(int iLumpIndex);
	int        getLumpIndex() const;

  protected:
	std::string          m_sName;
	std::vector<Vertex>  m_Vertexes;
	std::vector<Linedef> m_Linedef;
	std::vector<Thing>   m_Things;

	int m_XMin;
	int m_XMax;
	int m_YMin;
	int m_YMax;
	int m_iAutoMapScaleFactor;
	int m_iLumpIndex;

	Player *m_pPlayer;
};