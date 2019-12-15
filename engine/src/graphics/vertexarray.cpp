#include "vertexarray.h"

namespace prev {
	
	DEFINE_OBJECT_START(VertexArray, StrongHandle<VertexShader> vShader)
		DEFINE_OBJECT_BODY(VertexArray, vShader);
	DEFINE_OBJECT_END(VertexArray)

}