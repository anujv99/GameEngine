#pragma once

#include "core/window.h"

#include <GLFW/glfw3.h>

namespace prev {

	class GLFWWindow : public Window {
	public:
		GLFWWindow();
		~GLFWWindow();
	public:
		virtual bool Create(const WindowProps & props) override;
		virtual void Update() override;
	private:
		::GLFWwindow * m_Window;
	};

}