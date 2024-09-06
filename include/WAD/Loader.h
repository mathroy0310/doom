#pragma once

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

#include "DataTypes.h"

namespace WAD {
class Loader {
  public:
	Loader(std::string sWADFilePath);
	~Loader();

	bool LoadWAD();

  protected:
	bool OpenAndLoad();
	bool ReadDirectories();

	std::string            m_sFilePath;
	std::ifstream          m_File;
	uint8_t               *m_Data;
	std::vector<Directory> m_Directories;
};
} // namespace WAD