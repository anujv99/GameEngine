#include "application.h"

#include "core/window.h"

namespace prev {

	Application::Application() {
		LOG_WARN("Hello World");

		Window::CreateInst();
		Window::Ref().Create(WindowProps(1280, 720, "Game Engine Window"));
	}

	Application::~Application() {
		Window::DestroyInst();
	}

	void Application::Run() {
		while (true) {
			Window::Ref().Update();
		}
	}

}