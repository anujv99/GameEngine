#include "glfwwindow.h"

#include "events/applicationevent.h"
#include "events/keyevent.h"
#include "events/mouseevent.h"

#include "graphics/graphicscontext.h"

namespace prev {

	Window * Window::CreateWindow() {
		return dynamic_cast<Window *>(new prev::GLFWWindow());
	}

	GLFWWindow::GLFWWindow() : m_WindowProps({}) {}

	GLFWWindow::~GLFWWindow() {
		glfwDestroyWindow(m_WindowProps.Window);
		glfwTerminate();
		DLOG_INFO("Window Destroyed");
	}

	void GLFWWindow::Create(const WindowProps & props) {
		ASSERTM(glfwInit(), "Failed to initialize GLFW");

		IF_DIRECTX(if (GraphicsContext::GetAPI() == GraphicsAPI::API_DIRECTX) {
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		});

		::GLFWwindow * window = glfwCreateWindow(props.GetWidth(), props.GetHeight(), props.GetTitle().c_str(), nullptr, nullptr);

		ASSERTM(window, "Failed to create GLFW window");

		m_WindowProps.Window = window;
		m_WindowProps.Width = props.GetWidth();
		m_WindowProps.Height = props.GetHeight();
		m_WindowProps.Title = props.GetTitle();

		glfwSetWindowUserPointer(window, reinterpret_cast<void *>(this));

		m_WindowProps.EventFunction = [](Event & e) -> void {
			DLOG_INFO("%s", e.ToString().c_str());
		};

		SetCallbacks();

		DLOG_INFO("Window Created. Size : [%d, %d], Title : %s", m_WindowProps.Width, m_WindowProps.Height, m_WindowProps.Title.c_str());
	}

	void GLFWWindow::Update() {
		if (!glfwWindowShouldClose(m_WindowProps.Window)) {
			glfwPollEvents();
			glfwSwapBuffers(m_WindowProps.Window);
		}
	}

	void GLFWWindow::SetEventFunction(EventFunc eventFunc) {
		if (eventFunc)
			m_WindowProps.EventFunction = eventFunc;
	}

	void GLFWWindow::SetCallbacks() {

		glfwSetWindowCloseCallback(m_WindowProps.Window, [](::GLFWwindow * window) -> void {
			GLFWWindow * win = reinterpret_cast<GLFWWindow *>(glfwGetWindowUserPointer(window));
			WindowCloseEvent e;
			win->m_WindowProps.EventFunction(e);
		});

		glfwSetWindowSizeCallback(m_WindowProps.Window, [](::GLFWwindow * window, int width, int height) -> void {
			GLFWWindow * win = reinterpret_cast<GLFWWindow *>(glfwGetWindowUserPointer(window));
			WindowResizeEvent e(static_cast<pvint>(width), static_cast<pvint>(height));
			win->m_WindowProps.EventFunction(e);
		});

		glfwSetWindowPosCallback(m_WindowProps.Window, [](::GLFWwindow * window, int x, int y) -> void {
			GLFWWindow * win = reinterpret_cast<GLFWWindow *>(glfwGetWindowUserPointer(window));
			WindowMoveEvent e(static_cast<pvint>(x), static_cast<pvint>(y));
			win->m_WindowProps.EventFunction(e);
		});

		glfwSetKeyCallback(m_WindowProps.Window, [](::GLFWwindow * window, int keyCode, int scanCode, int action, int mods) -> void {
			GLFWWindow * win = reinterpret_cast<GLFWWindow *>(glfwGetWindowUserPointer(window));
			switch (action) {
			case GLFW_PRESS:
			{
				KeyPressedEvent e(static_cast<pvint>(keyCode), false);
				win->m_WindowProps.EventFunction(e);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent e(static_cast<pvint>(keyCode), true);
				win->m_WindowProps.EventFunction(e);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent e(static_cast<pvint>(keyCode));
				win->m_WindowProps.EventFunction(e);
				break;
			}
			}
		});

		glfwSetMouseButtonCallback(m_WindowProps.Window, [](::GLFWwindow * window, int button, int action, int mods) -> void {
			GLFWWindow * win = reinterpret_cast<GLFWWindow *>(glfwGetWindowUserPointer(window));
			switch (action) {
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent e(static_cast<pvint>(button));
				win->m_WindowProps.EventFunction(e);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent e(static_cast<pvint>(button));
				win->m_WindowProps.EventFunction(e);
				break;
			}
			}
		});

		glfwSetCursorPosCallback(m_WindowProps.Window, [](::GLFWwindow * window, double x, double y) -> void {
			GLFWWindow * win = reinterpret_cast<GLFWWindow *>(glfwGetWindowUserPointer(window));
			MouseMovedEvent e(static_cast<pvfloat>(x), static_cast<pvfloat>(y));
			win->m_WindowProps.EventFunction(e);
		});

		glfwSetScrollCallback(m_WindowProps.Window, [](::GLFWwindow * window, double xOffset, double yOffset) -> void {
			GLFWWindow * win = reinterpret_cast<GLFWWindow *>(glfwGetWindowUserPointer(window));
			MouseMovedEvent e(static_cast<pvfloat>(xOffset), static_cast<pvfloat>(yOffset));
			win->m_WindowProps.EventFunction(e);
		});

		glfwSetCharCallback(m_WindowProps.Window, [](::GLFWwindow * window, unsigned int character) -> void {
			GLFWWindow * win = reinterpret_cast<GLFWWindow *>(glfwGetWindowUserPointer(window));
			CharacterEvent e(static_cast<pvuchar>(character));
			win->m_WindowProps.EventFunction(e);
		});

	}

}