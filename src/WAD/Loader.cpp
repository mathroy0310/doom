#include "WAD/Loader.h"
#include <iostream>

namespace WAD {

WAD::Loader::Loader() : m_Data(nullptr) {}

WAD::Loader::~Loader() {
	delete[] m_Data;
}

void WAD::Loader::setWADFilePath(std::string sFilePath) { m_sFilePath = sFilePath; }

bool WAD::Loader::loadWAD() {
	if (!openAndLoad()) {
		return false;
	}

	if (!readDirectories()) {
		return false;
	}

	return true;
}

bool WAD::Loader::openAndLoad() {
	std::cout << "INFO: Loading WAD file: " << m_sFilePath << std::endl;

	m_File.open(m_sFilePath, std::ifstream::binary);
	if (!m_File.is_open()) {
		std::cout << "Error: Failed to open WAD file" << m_sFilePath << std::endl;
		return false;
	}

	m_File.seekg(0, m_File.end);
	size_t length = m_File.tellg();

	if (m_Data) {
		delete[] m_Data;
	}

	m_Data = new uint8_t[length];
	if (m_Data == nullptr) {
		std::cout << "Error: Failed alocate memory for WAD file of size " << length << std::endl;
		return false;
	}

	m_File.seekg(std::ifstream::beg);
	m_File.read((char *) m_Data, length);

	m_File.close();

	std::cout << "INFO: Loading complete." << std::endl;
	return true;
}

bool WAD::Loader::readDirectories() {
	Header header;
	m_Reader.readHeaderData(m_Data, 0, header);

	// std::cout << header.WADType << std::endl;
	// std::cout << header.DirectoryCount << std::endl;
	// std::cout << header.DirectoryOffset << std::endl;
	// std::cout << std::endl << std::endl;

	Directory directory;

	for (unsigned int i = 0; i < header.DirectoryCount; ++i) {
		m_Reader.readDirectoryData(m_Data, header.DirectoryOffset + i * 16, directory);

		m_Directories.push_back(directory);

		// std::cout << directory.LumpOffset << std::endl;
		// std::cout << directory.LumpSize << std::endl;
		// std::cout << directory.LumpName << std::endl;
		// std::cout << std::endl;
	}

	return true;
}

bool WAD::Loader::loadMapData(Map *pMap) {
	std::cout << "INFO: Parsing Map: " << pMap->getName() << std::endl;

	std::cout << "INFO: Processing Map Vertex" << std::endl;
	if (!readMapVertexes(pMap)) {
		std::cerr << "Error: Failed to read map vertex from MAP: " << pMap->getName() << std::endl;
		return false;
	}
	std::cout << "INFO: Processing Map Sectors" << std::endl;
	if (!readMapSectors(pMap)) {
		std::cout << "Error: Failed to load map sectors data MAP: " << pMap->getName() << std::endl;
		return false;
	}
	std::cout << "INFO: Processing Map Sidedefs" << std::endl;
	if (!readMapSidedefs(pMap)) {
		std::cout << "Error: Failed to load map Sidedefs data MAP: " << pMap->getName() << std::endl;
		return false;
	}
	std::cout << "INFO: Processing Map Linedef" << std::endl;
	if (!readMapLinedefs(pMap)) {
		std::cerr << "Error: Failed to read map linedef from MAP: " << pMap->getName() << std::endl;
		return false;
	}
	std::cout << "INFO: Processing Map Segs" << std::endl;
	if (!readMapSegs(pMap)) {
		std::cout << "Error: Failed to load map segs data MAP: " << pMap->getName() << std::endl;
		return false;
	}
	std::cout << "INFO: Processing Map Things" << std::endl;
	if (!readMapThings(pMap)) {
		std::cerr << "Error: Failed to load map thing data MAP: " << pMap->getName() << std::endl;
		return false;
	}
	std::cout << "INFO: Processing Map Nodes" << std::endl;
	if (!readMapNodes(pMap)) {
		std::cerr << "Error: Failed to load map node data MAP: " << pMap->getName() << std::endl;
		return false;
	}
	std::cout << "INFO: Processing Map Subsectors" << std::endl;
	if (!readMapSubsectors(pMap)) {
		std::cerr << "Error: Failed to load map subsectors data MAP: " << pMap->getName() << std::endl;
		return false;
	}

	return true;
}

int WAD::Loader::findMapIndex(Map *pMap) {
	if (pMap->getLumpIndex() > -1) {
		return pMap->getLumpIndex();
	}

	for (unsigned int i = 0; i < m_Directories.size(); ++i) {
		if (m_Directories[i].LumpName == pMap->getName()) {
			pMap->setLumpIndex(i);
			return i;
		}
	}

	return -1;
}

bool WAD::Loader::readMapVertexes(Map *pMap) {
	int iMapIndex = findMapIndex(pMap);

	if (iMapIndex == -1) {
		return false;
	}

	iMapIndex += EMAPLUMPSINDEX::eVERTEXES;

	if (std::strcmp(m_Directories[iMapIndex].LumpName, "VERTEXES") != 0) {
		return false;
	}

	int iVertexSizeInBytes = sizeof(Vertex);
	int iVertexCount = m_Directories[iMapIndex].LumpSize / iVertexSizeInBytes;

	Vertex vertex;
	for (int i = 0; i < iVertexCount; ++i) {
		m_Reader.readVertexData(m_Data, m_Directories[iMapIndex].LumpOffset + i * iVertexSizeInBytes, vertex);

		pMap->addVertex(vertex);

		// std::cout << "(" << vertex.XPosition << "," << vertex.YPosition << ")" << std::endl;
		//  std::cout << std::endl;
	}

	return true;
}

bool WAD::Loader::readMapSectors(Map *pMap) {
	int iMapIndex = findMapIndex(pMap);

	if (iMapIndex == -1) {
		return false;
	}

	iMapIndex += EMAPLUMPSINDEX::eSECTORS;

	if (std::strcmp(m_Directories[iMapIndex].LumpName, "SECTORS") != 0) {
		return false;
	}

	int iSectorSizeInBytes = sizeof(WADSector);
	int iSectorsCount = m_Directories[iMapIndex].LumpSize / iSectorSizeInBytes;

	WADSector sector;
	for (int i = 0; i < iSectorsCount; ++i) {
		m_Reader.readSectorData(m_Data, m_Directories[iMapIndex].LumpOffset + i * iSectorSizeInBytes, sector);
		pMap->addSector(sector);
	}

	return true;
}

bool WAD::Loader::readMapSidedefs(Map *pMap) {
	int iMapIndex = findMapIndex(pMap);

	if (iMapIndex == -1) {
		return false;
	}

	iMapIndex += EMAPLUMPSINDEX::eSIDEDDEFS;

	if (std::strcmp(m_Directories[iMapIndex].LumpName, "SIDEDEFS") != 0) {
		return false;
	}

	int iSidedefSizeInBytes = sizeof(WADSidedef);
	int iSidedefsCount = m_Directories[iMapIndex].LumpSize / iSidedefSizeInBytes;

	WADSidedef sidedef;
	for (int i = 0; i < iSidedefsCount; ++i) {
		m_Reader.readSidedefData(m_Data, m_Directories[iMapIndex].LumpOffset + i * iSidedefSizeInBytes, sidedef);
		pMap->addSidedef(sidedef);
	}

	return true;
}

bool WAD::Loader::readMapThings(Map *pMap) {
	int iMapIndex = findMapIndex(pMap);
	if (iMapIndex == -1) {
		return false;
	}

	iMapIndex += EMAPLUMPSINDEX::eTHINGS;

	if (std::strcmp(m_Directories[iMapIndex].LumpName, "THINGS") != 0) {
		return false;
	}

	int iThingsSizeInBytes = sizeof(Thing);
	int iThingsCount = m_Directories[iMapIndex].LumpSize / iThingsSizeInBytes;

	Thing thing;
	for (int i = 0; i < iThingsCount; ++i) {
		m_Reader.readThingData(m_Data, m_Directories[iMapIndex].LumpOffset + i * iThingsSizeInBytes, thing);
		(pMap->getThings())->addThing(thing);

		// std::cout << thing.XPosition << std::endl;
		// std::cout << thing.YPosition << std::endl;
		// std::cout << thing.Angle << std::endl;
		// std::cout << thing.Type << std::endl;
		// std::cout << thing.Flags << std::endl;
		// std::cout << std::endl;
	}
	return true;
}

bool WAD::Loader::readMapLinedefs(Map *pMap) {
	int iMapIndex = findMapIndex(pMap);
	if (iMapIndex == -1) {
		return false;
	}

	iMapIndex += EMAPLUMPSINDEX::eLINEDEFS;

	if (std::strcmp(m_Directories[iMapIndex].LumpName, "LINEDEFS") != 0) {
		return false;
	}

	int iLinedefSizeInBytes = sizeof(WADLinedef);
	int iLinedefCount = m_Directories[iMapIndex].LumpSize / iLinedefSizeInBytes;

	WADLinedef linedef;
	for (int i = 0; i < iLinedefCount; ++i) {
		m_Reader.readLinedefData(m_Data, m_Directories[iMapIndex].LumpOffset + i * iLinedefSizeInBytes, linedef);

		pMap->addLinedef(linedef);

		// std::cout << linedef.StartVertex << std::endl;
		// std::cout << linedef.EndVertex << std::endl;
		// std::cout << linedef.Flags << std::endl;
		// std::cout << linedef.LineType << std::endl;
		// std::cout << linedef.SectorTag << std::endl;
		// std::cout << linedef.RightSidedef << std::endl;
		// std::cout << linedef.LeftSidedef << std::endl;

		// std::cout << std::endl;
	}

	return true;
}

bool WAD::Loader::readMapNodes(Map *pMap) {
	int iMapIndex = findMapIndex(pMap);

	if (iMapIndex == -1) {
		return false;
	}

	iMapIndex += EMAPLUMPSINDEX::eNODES;

	if (std::strcmp(m_Directories[iMapIndex].LumpName, "NODES") != 0) {
		return false;
	}

	int iNodesSizeInBytes = sizeof(Node);
	int iNodesCount = m_Directories[iMapIndex].LumpSize / iNodesSizeInBytes;

	Node node;
	for (int i = 0; i < iNodesCount; ++i) {
		m_Reader.readNodeData(m_Data, m_Directories[iMapIndex].LumpOffset + i * iNodesSizeInBytes, node);

		pMap->addNode(node);
	}

	return true;
}

bool WAD::Loader::readMapSubsectors(Map *pMap) {
	int iMapIndex = findMapIndex(pMap);

	if (iMapIndex == -1) {
		return false;
	}

	iMapIndex += EMAPLUMPSINDEX::eSSECTORS;

	if (std::strcmp(m_Directories[iMapIndex].LumpName, "SSECTORS") != 0) {
		return false;
	}

	int iSubsectorsSizeInBytes = sizeof(Subsector);
	int iSubsectorsCount = m_Directories[iMapIndex].LumpSize / iSubsectorsSizeInBytes;

	Subsector subsector;
	for (int i = 0; i < iSubsectorsCount; ++i) {
		m_Reader.readSubsectorData(m_Data, m_Directories[iMapIndex].LumpOffset + i * iSubsectorsSizeInBytes, subsector);

		pMap->addSubsector(subsector);
	}

	return true;
}

bool WAD::Loader::readMapSegs(Map *pMap) {
	int iMapIndex = findMapIndex(pMap);

	if (iMapIndex == -1) {
		return false;
	}

	iMapIndex += EMAPLUMPSINDEX::eSEAGS;

	if (std::strcmp(m_Directories[iMapIndex].LumpName, "SEGS") != 0) {
		return false;
	}

	int iSegsSizeInBytes = sizeof(WADSeg);
	int iSegsCount = m_Directories[iMapIndex].LumpSize / iSegsSizeInBytes;

	WADSeg seg;
	for (int i = 0; i < iSegsCount; ++i) {
		m_Reader.readSegData(m_Data, m_Directories[iMapIndex].LumpOffset + i * iSegsSizeInBytes, seg);

		pMap->addSeg(seg);
	}

	return true;
}

} // namespace WAD