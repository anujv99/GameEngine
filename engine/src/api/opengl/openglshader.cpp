#include "openglshader.h"

#ifdef ENGINE_OPENGL

namespace prev {

	GLuint CreateAndCompileShader(GLenum shaderType, const pvstring & source) {
		ASSERT(source.size() > 0);

		GLuint id = glCreateShader(shaderType);
		ASSERT(id);

		const pvchar * src = source.c_str();
		pvint success;
		pvchar infoLog[512];

		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(id, sizeof(infoLog), nullptr, infoLog);
			switch (shaderType) {
			case GL_VERTEX_SHADER:
			{
				LOG_ERROR("Vertex Shader Compilation Failed:");
				break;
			}
			case GL_FRAGMENT_SHADER:
			{
				LOG_ERROR("Fragment Shader Compilation Failed:");
				break;
			}
			}
			LOG_ERROR("%s", infoLog);
			ASSERTM(false, "Fatal Error");
		}

		return id;
	}

	// ------------- VERTEX SHADER -------------

	DEFINE_OBJECT_OPENGL(VertexShader, const pvstring & source) {
		return dynamic_cast<VertexShader *>(new OpenGLVertexShader(source));
	}

	OpenGLVertexShader::OpenGLVertexShader(const pvstring & source) : m_ID(0u) {
		m_ID = CreateAndCompileShader(GL_VERTEX_SHADER, source);
		LOG_INFO("Vertex shader successfully created");
	}

	OpenGLVertexShader::~OpenGLVertexShader() {
		glDeleteShader(m_ID);
	}

	// ------------- FRAGMENT SHADER -------------

	DEFINE_OBJECT_OPENGL(FragmentShader, const pvstring & source) {
		return dynamic_cast<FragmentShader *>(new OpenGLFragmentShader(source));
	}

	OpenGLFragmentShader::OpenGLFragmentShader(const pvstring & source) {
		m_ID = CreateAndCompileShader(GL_FRAGMENT_SHADER, source);
		LOG_INFO("Fragment shader successfully created");
	}

	OpenGLFragmentShader::~OpenGLFragmentShader() {
		glDeleteShader(m_ID);
	}

	// ------------- SHADER PROGRAM -------------

	DEFINE_OBJECT_OPENGL(ShaderProgram, StrongHandle<VertexShader> vShader, StrongHandle<FragmentShader> fShader) {
		return dynamic_cast<ShaderProgram *>(new OpenGLShaderProgram(vShader, fShader));
	}

	OpenGLShaderProgram::OpenGLShaderProgram(StrongHandle<VertexShader> vShader, StrongHandle<FragmentShader> fShader) : 
		m_ID(0u), m_VS(nullptr), m_FS(nullptr) {
		ASSERT(vShader != nullptr);
		ASSERT(fShader != nullptr);

		const OpenGLVertexShader * vs = dynamic_cast<const OpenGLVertexShader *>(vShader.Get());
		const OpenGLFragmentShader * fs = dynamic_cast<const OpenGLFragmentShader *>(fShader.Get());

		m_ID = glCreateProgram();
		glAttachShader(m_ID, vs->GetID());
		glAttachShader(m_ID, fs->GetID());
		glLinkProgram(m_ID);

		int success;
		char infoLog[512];

		glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
			LOG_ERROR("Shader Program Linking Failed:");
			LOG_ERROR("%s", infoLog);
			ASSERTM(false, "Fatal Error");
		}

		m_VS = vShader;
		m_FS = fShader;

		LOG_INFO("Shader program successfully created");
	}

	OpenGLShaderProgram::~OpenGLShaderProgram() {
		glDeleteProgram(m_ID);
	}

}

#endif