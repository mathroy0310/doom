#include "WAD/Reader.h"
#include <cstring>

namespace WAD {

WAD::Reader::Reader() {}

WAD::Reader::~Reader() {}

uint16_t WAD::Reader::read2Bytes(const uint8_t *pWADData, int offset) const {
	uint16_t readValue;
	std::memcpy(&readValue, pWADData + offset, sizeof(uint16_t));
	return readValue;
}

uint32_t WAD::Reader::read4Bytes(const uint8_t *pWADData, int offset) const {
	uint32_t readValue;
	std::memcpy(&readValue, pWADData + offset, sizeof(uint32_t));
	return readValue;
}

void WAD::Reader::readHeaderData(const uint8_t *pWADData, int offset, Header &header) {
	// 0x00 to 0x03
	header.WADType[0] = pWADData[offset];
	header.WADType[1] = pWADData[offset + 1];
	header.WADType[2] = pWADData[offset + 2];
	header.WADType[3] = pWADData[offset + 3];
	header.WADType[4] = '\0';

	// 0x04 to 0x07
	header.DirectoryCount = read4Bytes(pWADData, offset + 4);

	// 0x08 to 0x0b
	header.DirectoryOffset = read4Bytes(pWADData, offset + 8);
}

void WAD::Reader::readDirectoryData(const uint8_t *pWADData, int offset, Directory &directory) {
	// 0x00 to 0x03
	directory.LumpOffset = read4Bytes(pWADData, offset);

	// 0x04 to 0x07
	directory.LumpSize = read4Bytes(pWADData, offset + 4);

	// 0x08 to 0x0F
	directory.LumpName[0] = pWADData[offset + 8];
	directory.LumpName[1] = pWADData[offset + 9];
	directory.LumpName[2] = pWADData[offset + 10];
	directory.LumpName[3] = pWADData[offset + 11];
	directory.LumpName[4] = pWADData[offset + 12];
	directory.LumpName[5] = pWADData[offset + 13];
	directory.LumpName[6] = pWADData[offset + 14];
	directory.LumpName[7] = pWADData[offset + 15];
	directory.LumpName[8] = '\0';
}

void WAD::Reader::readVertexData(const uint8_t *pWADData, int offset, Vertex &vertex) {
	// 0x00 to 0x01
	vertex.XPosition = read2Bytes(pWADData, offset);
	// 0x02 to 0x03
	vertex.YPosition = read2Bytes(pWADData, offset + 2);
}

void WAD::Reader::readLinedefData(const uint8_t *pWADData, int offset, Linedef &linedef) {
	// 0x00 to 0x01
	linedef.StartVertex = read2Bytes(pWADData, offset);
	// 0x02 to 0x03
	linedef.EndVertex = read2Bytes(pWADData, offset + 2);
	// 0x04 to 0x05
	linedef.Flags = read2Bytes(pWADData, offset + 4);
	// 0x06 to 0x07
	linedef.LineType = read2Bytes(pWADData, offset + 6);
	// 0x08 to 0x09
	linedef.SectorTag = read2Bytes(pWADData, offset + 8);
	// 0x0a to 0x0b
	linedef.RightSidedef = read2Bytes(pWADData, offset + 10);
	// 0x0c to 0x0d
	linedef.LeftSidedef = read2Bytes(pWADData, offset + 12);
}

void WAD::Reader::readThingData(const uint8_t *pWADData, int offset, Thing &thing) {
	// 0x00 to 0x01
	thing.XPosition = read2Bytes(pWADData, offset);
	// 0x02 to 0x03
	thing.YPosition = read2Bytes(pWADData, offset + 2);
	// 0x04 to 0x05
	thing.Angle = read2Bytes(pWADData, offset + 4);
	// 0x06 to 0x07
	thing.Type = read2Bytes(pWADData, offset + 6);
	// 0x08 to 0x09
	thing.Flags = read2Bytes(pWADData, offset + 8);
}

void WAD::Reader::readNodesData(const uint8_t *pWADData, int offset, Node &node) {
	node.XPartition = read2Bytes(pWADData, offset);
	node.YPartition = read2Bytes(pWADData, offset + 2);
	node.ChangeXPartition = read2Bytes(pWADData, offset + 4);
	node.ChangeYPartition = read2Bytes(pWADData, offset + 6);

	node.RightBoxTop = read2Bytes(pWADData, offset + 8);
	node.RightBoxBottom = read2Bytes(pWADData, offset + 10);
	node.RightBoxLeft = read2Bytes(pWADData, offset + 12);
	node.RightBoxRight = read2Bytes(pWADData, offset + 14);

	node.LeftBoxTop = read2Bytes(pWADData, offset + 16);
	node.LeftBoxBottom = read2Bytes(pWADData, offset + 18);
	node.LeftBoxLeft = read2Bytes(pWADData, offset + 20);
	node.LeftBoxRight = read2Bytes(pWADData, offset + 22);

	node.RightChildID = read2Bytes(pWADData, offset + 24);
	node.LeftChildID = read2Bytes(pWADData, offset + 26);
}

} // namespace WAD