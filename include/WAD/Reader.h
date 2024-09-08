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
	void readSectorData(const uint8_t *pWADData, int offset, WADSector &sector);
	void readSidedefData(const uint8_t *pWADData, int offset, WADSidedef &sidedef);
	void readLinedefData(const uint8_t *pWADData, int offset, WADLinedef &linedef);
	void readThingData(const uint8_t *pWADData, int offset, Thing &thing);
	void readNodeData(const uint8_t *pWADData, int offset, Node &node);
	void readSubsectorData(const uint8_t *pWADData, int offset, Subsector &subsector);
	void readSegData(const uint8_t *pWADData, int offset, WADSeg &seg);

  protected:
	uint16_t read2Bytes(const uint8_t *pWADData, int offset) const;
	uint32_t read4Bytes(const uint8_t *pWADData, int offset) const;
};
} // namespace WAD