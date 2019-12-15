#include "openglcontext.h"

#ifdef ENGINE_OPENGL

#include <glad/glad.h>

namespace prev {

	GraphicsAPI GraphicsContext::s_GraphicsAPI = GraphicsAPI::API_OPENGL;

	GraphicsContext * GraphicsContext::CreateOpenGLContext() {
		return dynamic_cast<GraphicsContext *>(new OpenGLContext());
	}

	OpenGLContext::OpenGLContext() : m_WindowPtr(nullptr) {}

	OpenGLContext::~OpenGLContext() {}

	void OpenGLContext::CreateContext(pvptr rawWindowPtr) {
		m_WindowPtr = reinterpret_cast<::GLFWwindow *>(rawWindowPtr);
		ASSERTM(m_WindowPtr, "Unable to create OpenGL context without a valid window");

		glfwMakeContextCurrent(m_WindowPtr);
		int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
		ASSERTM(status, "Failed to initialize GLAD");

		DLOG_INFO("Successfully created OpenGL context");

		DLOG_INFO("OpenGL Info:");
		DLOG_INFO("\tVendor: %s", glGetString(GL_VENDOR));
		DLOG_INFO("\tRenderer: %s", glGetString(GL_RENDERER));
		DLOG_INFO("\tVersion: %s", glGetString(GL_VERSION));

		GLint versionMajor, versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		ASSERTM(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Cannot run engine below OpenGL version 4.5");
	}

	void OpenGLContext::BeginFrame() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLContext::EndFrame() {}

}

#endif