#pragma once

#ifdef ENGINE_OPENGL

#include "graphics/shader.h"

#include <glad/glad.h>

namespace prev {

	GLuint CreateAndCompileShader(GLenum shaderType, const pvstring & source);

	// ------------- VERTEX SHADER -------------

	class OpenGLVertexShader : public VertexShader {
	public:
		OpenGLVertexShader(const pvstring & source);
		~OpenGLVertexShader();

		inline GLuint GetID() const { PV_PROFILE_FUNCTION(); return m_ID; }
	private:
		GLuint m_ID;
	};

	// ------------- FRAGMENT SHADER -------------

	class OpenGLFragmentShader : public FragmentShader {
	public:
		OpenGLFragmentShader(const pvstring & source);
		~OpenGLFragmentShader();

		inline GLuint GetID() const { PV_PROFILE_FUNCTION(); return m_ID; }
	private:
		GLuint m_ID;
	};

	// ------------- SHADER PROGRAM -------------

	class OpenGLShaderProgram : public ShaderProgram {
	public:
		OpenGLShaderProgram(StrongHandle<VertexShader> vShader, StrongHandle<FragmentShader> fShader);
		~OpenGLShaderProgram();

		inline GLuint GetID() const { return m_ID; }

		virtual void Bind() override { PV_PROFILE_FUNCTION(); glUseProgram(m_ID); };
		virtual void UnBind() override { glUseProgram(0u); };
	private:
		GLuint m_ID;
		StrongHandle<VertexShader> m_VS;
		StrongHandle<FragmentShader> m_FS;
	};

}

#endif