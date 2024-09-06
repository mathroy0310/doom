#pragma once

#include <cstdint>

#include "DataTypes.h"

namespace WAD {
class Reader {
  public:
	Reader();
	~Reader();

	void readHeaderData(const uint8_t *pWADData, int offset, Header &header);
	void readDirectoryData(const uint8_t *pWADData, int offset, Directory &directory);
	void readVertexData(const uint8_t *pWADData, int offset, Vertex &vertex);
	void readLinedefData(const uint8_t *pWADData, int offset, Linedef &linedef);

  protected:
	uint16_t read2Bytes(const uint8_t *pWADData, int offset);
	uint32_t read4Bytes(const uint8_t *pWADData, int offset);
};
} // namespace WAD