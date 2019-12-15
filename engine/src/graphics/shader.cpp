#include "shader.h"

namespace prev {

	// ------------- VERTEX SHADER -------------

	DEFINE_OBJECT_START(VertexShader, const pvstring & source)
		DEFINE_OBJECT_BODY(VertexShader, source);
	DEFINE_OBJECT_END(VertexShader)

	// ------------- FRAGMENT SHADER -------------

	DEFINE_OBJECT_START(FragmentShader, const pvstring & source)
		DEFINE_OBJECT_BODY(FragmentShader, source);
	DEFINE_OBJECT_END(FragmentShader)
	
	// ------------- SHADER PROGRAM -------------

	DEFINE_OBJECT_START(ShaderProgram, StrongHandle<VertexShader> vShader, StrongHandle<FragmentShader> fShader)
		DEFINE_OBJECT_BODY(ShaderProgram, vShader, fShader);
	DEFINE_OBJECT_END(ShaderProgram)

}