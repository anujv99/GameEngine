#include "framebuffer.h"

namespace prev {

	DEFINE_OBJECT_START(Framebuffer, Vec2i size, TextureFormat format, FramebufferFlags flags)
		DEFINE_OBJECT_BODY(Framebuffer, size, format, flags);
	DEFINE_OBJECT_END(Framebuffer)

}