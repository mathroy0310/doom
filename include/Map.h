#pragma once

#include <string>
#include <vector>

#include "DataTypes.h"

class Map {
  public:
	Map(std::string sName);
	~Map();

	std::string getName();
	void        addVertex(Vertex &v);
	void        addLinedef(Linedef &l);

  protected:
	std::string          m_sName;
	std::vector<Vertex>  m_Vertexes;
	std::vector<Linedef> m_Linedef;
};