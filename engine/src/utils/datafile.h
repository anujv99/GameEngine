#pragma once

#include "../common/handledobject.h"
#include "../common/types.h"

#include "fileutils.h"

namespace prev {

	class DataFile : public HandledObject<DataFile> {
	public:
		DataFile();
		DataFile(const pvstring & path);
		~DataFile();
	public:
		bool OpenFile(const pvstring & path);
		pvstring ReadEntireFile();
	private:
		pvstring m_Content;
		pvstring m_FilePath;
		std::fstream m_File;
	};

}
