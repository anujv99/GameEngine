#pragma once

#ifdef ENGINE_OPENGL

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "graphics/graphicscontext.h"

namespace prev {

	class OpenGLContext : public GraphicsContext {
	public:
		OpenGLContext();
		~OpenGLContext();
	public:
		virtual void CreateContext(pvptr rawWindowPtr) override;
		virtual void BeginFrame() override;
		virtual void EndFrame() override;
	private:
		::GLFWwindow * m_WindowPtr;
	};

}

#endif