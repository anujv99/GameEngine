#include "datafile.h"

namespace prev {

	DataFile::DataFile() {}

	DataFile::DataFile(const pvstring & path) { OpenFile(path); }

	DataFile::~DataFile() {
		m_File.close();
	}

	bool DataFile::OpenFile(const pvstring & path) {
		if (!FileExists(path)) {
			LOG_FATAL("File not found : %s", path.c_str());
			return false;
		}

		m_File.open(path, std::ios::in | std::ios::binary);
		m_FilePath = path;
		m_Content.clear();
		return true;
	}

	pvstring DataFile::ReadEntireFile() {
		ASSERTM(m_File, "File not opened or is invalid");
		if (m_Content.size() != 0) { return m_Content; }
		m_File.seekg(0, std::ios::end);
		m_Content.resize(m_File.tellg());
		m_File.seekg(0, std::ios::beg);
		m_File.read(&m_Content[0], m_Content.size());
		m_File.close();
		return m_Content;
	}

}