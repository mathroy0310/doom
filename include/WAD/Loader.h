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
	bool readMapVertexes(Map *pMap);
	bool readMapLinedefs(Map *pMap);
	bool readMapThings(Map *pMap);
	bool readMapNodes(Map *pMap);
	bool readMapSubsectors(Map *pMap);
	bool readMapSectors(Map *pMap);
	bool readMapSidedefs(Map *pMap);
	bool readMapSegs(Map *pMap);

	int findMapIndex(Map *pMap);

  private:
	std::string            m_sFilePath;
	std::ifstream          m_File;
	uint8_t               *m_Data;
	std::vector<Directory> m_Directories;
	WAD::Reader            m_Reader;
};
} // namespace WAD