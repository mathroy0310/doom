#pragma once

#include <string>
#include <vector>

#include <SDL.h>


#include "DataTypes.h"

class Map {
  public:
	Map(std::string sName);
	~Map();

	std::string getName();
	void        addVertex(Vertex &v);
	void        addLinedef(Linedef &l);
	void        renderAutoMap(SDL_Renderer *pRenderer);

  protected:
	std::string          m_sName;
	std::vector<Vertex>  m_Vertexes;
	std::vector<Linedef> m_Linedef;

	int m_XMin;
	int m_XMax;
	int m_YMin;
	int m_YMax;
	int m_iAutoMapScaleFactor;
};