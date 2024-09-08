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

void WAD::Reader::readSectorData(const uint8_t *pWADData, int offset, WADSector &sector) {
	sector.FloorHeight = read2Bytes(pWADData, offset);
	sector.CeilingHeight = read2Bytes(pWADData, offset + 2);

	sector.FloorTexture[0] = pWADData[offset + 4];
	sector.FloorTexture[1] = pWADData[offset + 5];
	sector.FloorTexture[2] = pWADData[offset + 6];
	sector.FloorTexture[3] = pWADData[offset + 7];
	sector.FloorTexture[4] = pWADData[offset + 8];
	sector.FloorTexture[5] = pWADData[offset + 9];
	sector.FloorTexture[6] = pWADData[offset + 10];
	sector.FloorTexture[7] = pWADData[offset + 11];

	sector.CeilingTexture[0] = pWADData[offset + 12];
	sector.CeilingTexture[1] = pWADData[offset + 13];
	sector.CeilingTexture[2] = pWADData[offset + 14];
	sector.CeilingTexture[3] = pWADData[offset + 15];
	sector.CeilingTexture[4] = pWADData[offset + 16];
	sector.CeilingTexture[5] = pWADData[offset + 17];
	sector.CeilingTexture[6] = pWADData[offset + 18];
	sector.CeilingTexture[7] = pWADData[offset + 19];

	sector.Lightlevel = read2Bytes(pWADData, offset + 20);
	sector.Type = read2Bytes(pWADData, offset + 22);
	sector.Tag = read2Bytes(pWADData, offset + 24);
}

void WAD::Reader::readSidedefData(const uint8_t *pWADData, int offset, WADSidedef &sidedef) {
	sidedef.XOffset = read2Bytes(pWADData, offset);
	sidedef.YOffset = read2Bytes(pWADData, offset + 2);
	sidedef.UpperTexture[0] = pWADData[offset + 4];
	sidedef.UpperTexture[1] = pWADData[offset + 5];
	sidedef.UpperTexture[2] = pWADData[offset + 6];
	sidedef.UpperTexture[3] = pWADData[offset + 7];
	sidedef.UpperTexture[4] = pWADData[offset + 8];
	sidedef.UpperTexture[5] = pWADData[offset + 9];
	sidedef.UpperTexture[6] = pWADData[offset + 10];
	sidedef.UpperTexture[7] = pWADData[offset + 11];

	sidedef.LowerTexture[0] = pWADData[offset + 12];
	sidedef.LowerTexture[1] = pWADData[offset + 13];
	sidedef.LowerTexture[2] = pWADData[offset + 14];
	sidedef.LowerTexture[3] = pWADData[offset + 15];
	sidedef.LowerTexture[4] = pWADData[offset + 16];
	sidedef.LowerTexture[5] = pWADData[offset + 17];
	sidedef.LowerTexture[6] = pWADData[offset + 18];
	sidedef.LowerTexture[7] = pWADData[offset + 19];

	sidedef.MiddleTexture[0] = pWADData[offset + 20];
	sidedef.MiddleTexture[1] = pWADData[offset + 21];
	sidedef.MiddleTexture[2] = pWADData[offset + 22];
	sidedef.MiddleTexture[3] = pWADData[offset + 23];
	sidedef.MiddleTexture[4] = pWADData[offset + 24];
	sidedef.MiddleTexture[5] = pWADData[offset + 25];
	sidedef.MiddleTexture[6] = pWADData[offset + 26];
	sidedef.MiddleTexture[7] = pWADData[offset + 27];

	sidedef.SectorID = read2Bytes(pWADData, offset + 28);
}

void WAD::Reader::readLinedefData(const uint8_t *pWADData, int offset, WADLinedef &linedef) {
	linedef.StartVertexID = read2Bytes(pWADData, offset);
	linedef.EndVertexID = read2Bytes(pWADData, offset + 2);
	linedef.Flags = read2Bytes(pWADData, offset + 4);
	linedef.LineType = read2Bytes(pWADData, offset + 6);
	linedef.SectorTag = read2Bytes(pWADData, offset + 8);
	linedef.RightSidedef = read2Bytes(pWADData, offset + 10);
	linedef.LeftSidedef = read2Bytes(pWADData, offset + 12);
}

void WAD::Reader::readThingData(const uint8_t *pWADData, int offset, Thing &thing) {
	thing.XPosition = read2Bytes(pWADData, offset);
	thing.YPosition = read2Bytes(pWADData, offset + 2);
	thing.Angle = read2Bytes(pWADData, offset + 4);
	thing.Type = read2Bytes(pWADData, offset + 6);
	thing.Flags = read2Bytes(pWADData, offset + 8);
}

void WAD::Reader::readNodeData(const uint8_t *pWADData, int offset, Node &node) {
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

void WAD::Reader::readSubsectorData(const uint8_t *pWADData, int offset, Subsector &subsector) {
	subsector.SegCount = read2Bytes(pWADData, offset);
	subsector.FirstSegID = read2Bytes(pWADData, offset + 2);
}

void WAD::Reader::readSegData(const uint8_t *pWADData, int offset, WADSeg &seg) {
	seg.StartVertexID = read2Bytes(pWADData, offset);
	seg.EndVertexID = read2Bytes(pWADData, offset + 2);
	seg.SlopeAngle = read2Bytes(pWADData, offset + 4);
	seg.LinedefID = read2Bytes(pWADData, offset + 6);
	seg.Direction = read2Bytes(pWADData, offset + 8);
	seg.Offset = read2Bytes(pWADData, offset + 10);
}

} // namespace WAD