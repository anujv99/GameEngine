
#include <fstream>

#include "../common/types.h"

namespace prev {

	static bool FileExists(const pvstring & path) {
		std::ifstream file(path);
		return file.operator bool();
	}

}