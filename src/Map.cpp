#include "Map.h"

Map::Map(std::string sName) : m_sName(sName)
{
}

Map::~Map()
{
}

void Map::addVertex(Vertex &v)
{
    m_Vertexes.push_back(v);
}

void Map::addLinedef(Linedef &l)
{
    m_Linedef.push_back(l);
}

std::string Map::getName()
{
    return m_sName;
}