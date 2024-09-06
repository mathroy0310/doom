#include "WAD/Loader.h"
#include "WAD/Reader.h"
#include <iostream>

namespace WAD {

WAD::Loader::Loader() : m_Data(NULL) {}

WAD::Loader::~Loader() {
	if (m_Data) delete[] m_Data;
	m_Data = NULL;
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
		m_Data = NULL;
	}

	m_Data = new uint8_t[length];
	if (m_Data == NULL) {
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
	if (!readMapVertex(pMap)) {
		std::cout << "Error: Failed to read map vertex from MAP: " << pMap->getName() << std::endl;
		return false;
	}
	std::cout << "INFO: Processing Map Linedef" << std::endl;
	if (!readMapLinedef(pMap)) {
		std::cout << "Error: Failed to read map linedef from MAP: " << pMap->getName() << std::endl;
		return false;
	}
	std::cout << "INFO: Processing Map Things" << std::endl;
	if (!readMapThing(pMap)) {
		std::cout << "Error: Failed to load map thing data MAP: " << pMap->getName() << std::endl;
		return false;
	}
	std::cout << "INFO: Processing Map Nodes" << std::endl;
	if (!readMapNodes(pMap)) {
		std::cout << "Error: Failed to load map node data MAP: " << pMap->getName() << std::endl;
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

bool WAD::Loader::readMapVertex(Map *pMap) {
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

bool WAD::Loader::readMapLinedef(Map *pMap) {
	int iMapIndex = findMapIndex(pMap);
	if (iMapIndex == -1) {
		return false;
	}

	iMapIndex += EMAPLUMPSINDEX::eLINEDEFS;

	if (std::strcmp(m_Directories[iMapIndex].LumpName, "LINEDEFS") != 0) {
		return false;
	}

	int iLinedefSizeInBytes = sizeof(Linedef);
	int iLinedefCount = m_Directories[iMapIndex].LumpSize / iLinedefSizeInBytes;

	Linedef linedef;
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

bool WAD::Loader::readMapThing(Map *pMap) {
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
	for (int i = 0; i < iThingsCount; i++) {
		m_Reader.readThingData(m_Data, m_Directories[iMapIndex].LumpOffset + i * iThingsSizeInBytes, thing);

		pMap->addThing(thing);

		// std::cout << thing.XPosition << std::endl;
		// std::cout << thing.YPosition << std::endl;
		// std::cout << thing.Angle << std::endl;
		// std::cout << thing.Type << std::endl;
		// std::cout << thing.Flags << std::endl;
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
		m_Reader.readNodesData(m_Data, m_Directories[iMapIndex].LumpOffset + i * iNodesSizeInBytes, node);

		pMap->addNode(node);
	}

	return true;
}

} // namespace WAD