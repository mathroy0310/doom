#pragma once

#include <cstdint>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

#include "DataTypes.h"
#include "DisplayManager.h"
#include "Map.h"
#include "WAD/Reader.h"

namespace WAD {
class Loader {
  public:
	Loader();
	~Loader();

	void setFilePath(std::string sFilePath);

	bool loadToMemory();
	bool loadMapData(Map *pMap);
	bool loadPalette(DisplayManager *pDisplayManager);

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
	int findLumpByName(const std::string &LumpName);

  private:
	std::string            m_sFilePath;
	std::ifstream          m_File;
	uint8_t               *m_pData;
	std::vector<Directory> m_Directories;
	WAD::Reader            m_Reader;
};
} // namespace WAD