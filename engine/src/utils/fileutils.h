
#include <fstream>

#include "../common/types.h"

namespace prev {
	class FileUtils {
	public:
		static bool FileExists(const pvstring & path) {
			std::ifstream file(path.c_str());
			return file.operator bool();
		}
	};
}