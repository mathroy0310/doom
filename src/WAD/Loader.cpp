#include "WAD/Loader.h"
#include "WAD/Reader.h"
#include <iostream>

namespace WAD {

WAD::Loader::Loader(std::string sFilePath) : m_Data(NULL), m_sFilePath(sFilePath) {}

WAD::Loader::~Loader() {
    if (m_Data)
	    delete[] m_Data;
	m_Data = NULL;
}

bool WAD::Loader::LoadWAD() {
	if (!OpenAndLoad()) {
		return false;
	}

	if (!ReadDirectories()) {
		return false;
	}

	return true;
}

bool WAD::Loader::OpenAndLoad() {
	std::cout << "Info: Loading WAD file: " << m_sFilePath << std::endl;

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

	std::cout << "Info: Loading complete." << std::endl;
	return true;
}

bool WAD::Loader::ReadDirectories() {
	WAD::Reader reader;

	Header header;
	reader.ReadHeaderData(m_Data, 0, header);

	std::cout << header.WADType << std::endl;
	std::cout << header.DirectoryCount << std::endl;
	std::cout << header.DirectoryOffset << std::endl;
	std::cout << std::endl << std::endl;

	Directory directory;

	for (unsigned int i = 0; i < header.DirectoryCount; ++i) {
		reader.ReadDirectoryData(m_Data, header.DirectoryOffset + i * 16, directory);

		m_Directories.push_back(directory);

		std::cout << directory.LumpOffset << std::endl;
		std::cout << directory.LumpSize << std::endl;
		std::cout << directory.LumpName << std::endl;
		std::cout << std::endl;
	}

	return true;
}

} // namespace WAD