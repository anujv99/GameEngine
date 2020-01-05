
#include <fstream>

#include "../common/types.h"

namespace prev {

	static bool FileExists(const pvstring & path) {
		std::ifstream file(path.c_str());
		return file.operator bool();
	}

}