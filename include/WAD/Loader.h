#pragma once

#include <cstdint>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

#include "DataTypes.h"
#include "Map.h"
#include "WAD/Reader.h"

namespace WAD {
class Loader {
  public:
	Loader();
	~Loader();

	void setWADFilePath(std::string sFilePath);

	bool loadWAD();
	bool loadMapData(Map *pMap);

  protected:
	bool openAndLoad();
	bool readDirectories();
	bool readMapVertex(Map *pMap);
	bool readMapLinedef(Map *pMap);
	bool readMapThing(Map *pMap);
	int  findMapIndex(Map *pMap);

	std::string            m_sFilePath;
	std::ifstream          m_File;
	uint8_t               *m_Data;
	std::vector<Directory> m_Directories;
	WAD::Reader            m_Reader;
};
} // namespace WAD