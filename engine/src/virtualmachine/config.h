
#include "../common/types.h"

namespace prev {

	struct Config {
		pvuint WindowWidth = 1280u, WindowHeight = 720u;
		pvstring WindowTitle = "Engine WIndow";
		pvstring GraphicsAPI = "OPENGL";
		pvstring ResFolderDir = "";
	};

}
