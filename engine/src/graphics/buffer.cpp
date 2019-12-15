#include "buffer.h"

namespace prev {

	DEFINE_OBJECT_START(VertexBuffer, const void * data, pvsizet size, pvuint stride, BufferUsage usage)
		DEFINE_OBJECT_BODY(VertexBuffer, data, size, stride, usage);
	DEFINE_OBJECT_END(VertexBuffer)

}