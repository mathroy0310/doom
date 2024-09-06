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
	Loader(std::string sWADFilePath);
	~Loader();

	bool loadWAD();
	bool loadMapData(Map &map);

  protected:
	bool openAndLoad();
	bool readDirectories();
	bool readMapVertex(Map &map);
	bool readMapLinedef(Map &map);
	int  findMapIndex(Map &map);

	std::string            m_sFilePath;
	std::ifstream          m_File;
	uint8_t               *m_Data;
	std::vector<Directory> m_Directories;
	WAD::Reader            m_Reader;
};
} // namespace WAD