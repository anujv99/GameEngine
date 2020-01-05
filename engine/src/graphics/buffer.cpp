#include "buffer.h"

namespace prev {

	// ------------- VERTEX BUFFER -------------

	DEFINE_OBJECT_START(VertexBuffer, const void * data, pvsizet size, pvuint stride, BufferUsage usage)
		DEFINE_OBJECT_BODY(VertexBuffer, data, size, stride, usage);
	DEFINE_OBJECT_END(VertexBuffer)

	// ------------- UNIFORM BUFFER -------------

	DEFINE_OBJECT_START(UniformBuffer, const void * data, pvsizet size, BufferUsage usage)
		DEFINE_OBJECT_BODY(UniformBuffer, data, size, usage);
	DEFINE_OBJECT_END(UniformBuffer)

}