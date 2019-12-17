#include "application.h"

#include "core/window.h"

#include "graphics/graphicscontext.h"
#include "graphics/renderstate.h"

#include "graphics/buffer.h"
#include "graphics/shader.h"
#include "graphics/bufferlayout.h"
#include "graphics/vertexarray.h"

#include "renderer/renderer2d.h"

namespace prev {

	Application::Application() : m_IsWindowOpen(true) {
		PV_PROFILE_BEGIN_SESSION("EveryThing", "gg.json");

		PV_PROFILE_FUNCTION();

		GraphicsContext::SetGraphicsAPI(GraphicsAPI::API_OPENGL);

		Window::CreateInst();
		Window::Ref().Create(WindowProps(1280, 720, "Game Engine Window"));
		Window::Ref().SetEventFunction(BIND_EVENT_FN(Application::OnEvent));

		GraphicsContext::CreateInst();
		GraphicsContext::Ref().CreateContext(Window::Ref().GetRawWindowPtr());

		RenderState::CreateInst();
		RenderState::Ref().SetTopology(PrimitiveTopology::TOPOLOGY_TRIANGLE);
		RenderState::Ref().SetViewport({ 0.0f, 0.0f, 1280.0f, 720.0f, 0.0f, 1.0f });

		Renderer2D::CreateInst();
	}

	Application::~Application() {
		Renderer2D::DestroyInst();
		GraphicsContext::DestroyInst();
		Window::DestroyInst();

		PV_PROFILE_END_SESSION();
	}

	void Application::Run() {
		while (m_IsWindowOpen) {

			PV_PROFILE_FUNCTION();

			GraphicsContext::Ref().BeginFrame();

			for (auto & layer : m_LayerStack) {
				layer->OnUpdate(0.0f);
			}

			Renderer2D::Ref().Render();

			GraphicsContext::Ref().EndFrame();

			Window::Ref().Update();
		}
	}

	void Application::OnEvent(Event & e) {
		EventDispatcher dispatcher(e);
		
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

		for (auto & layer : m_LayerStack) {
			layer->OnEvent(e);
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent & e) {
		m_IsWindowOpen = false;
		return false;
	}

}