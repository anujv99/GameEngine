#include "texture.h"

namespace prev {

	DEFINE_OBJECT_START(Texture2D, Vec2i size, TextureParams params)
		DEFINE_OBJECT_BODY(Texture2D, size, params);
	DEFINE_OBJECT_END(Texture2D)

}