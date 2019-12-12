#include "glfwwindow.h"

namespace prev {

	Window * Window::CreateWindow() {
		return dynamic_cast<Window *>(new prev::GLFWWindow());
	}

	GLFWWindow::GLFWWindow() : m_Window(nullptr) {}

	GLFWWindow::~GLFWWindow() {
		
	}

	bool GLFWWindow::Create(const WindowProps & props) {
		if (!glfwInit()) {
			LOG_ERROR("Unable to initialize GLFW");
			return false;
		}

		::GLFWwindow * window = glfwCreateWindow(props.GetWidth(), props.GetHeight(), props.GetTitle().c_str(), nullptr, nullptr);

		if (!window) {
			LOG_ERROR("Unable to create GLFW window");
			return false;
		}

		m_Window = window;

		return true;
	}

	void GLFWWindow::Update() {
		if (!glfwWindowShouldClose(m_Window)) {
			glfwPollEvents();
			glfwSwapBuffers(m_Window);
		}
	}

}