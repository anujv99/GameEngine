#include "application.h"

#include "core/window.h"
#include "core/timer.h"

#include "graphics/graphicscontext.h"
#include "graphics/renderstate.h"

#include "graphics/buffer.h"
#include "graphics/shader.h"
#include "graphics/bufferlayout.h"
#include "graphics/vertexarray.h"

#include "renderer/renderer2d.h"

#include "virtualmachine/virtualmachine.h"

namespace prev {
	
	pvuint NUMBER_OF_DRAW_CALLS = 0u;

	Application::Application() : m_IsWindowOpen(true) {
		
		PV_PROFILE_BEGIN_SESSION("EveryThing", "gg.json");

		PV_PROFILE_FUNCTION();

		VirtualMachine::CreateInst();

		Config conf = VirtualMachine::Ref().ReadConfigFile("../../../../res/scripts/config.lua");

		if (conf.GraphicsAPI == "OPENGL")
			GraphicsContext::SetGraphicsAPI(GraphicsAPI::API_OPENGL);
		else if (conf.GraphicsAPI == "DIRECTX")
			GraphicsContext::SetGraphicsAPI(GraphicsAPI::API_DIRECTX);
		else {
			LOG_ERROR("Invalid graphics api in config file. Using default api : OpenGL");
			GraphicsContext::SetGraphicsAPI(GraphicsAPI::API_OPENGL);
		}

		Window::CreateInst();
		Window::Ref().Create(WindowProps(conf.WindowWidth, conf.WindowHeight, conf.WindowTitle));
		Window::Ref().SetEventFunction(BIND_EVENT_FN(Application::OnEvent));

		GraphicsContext::CreateInst();
		GraphicsContext::Ref().CreateContext(Window::Ref().GetRawWindowPtr());

		RenderState::CreateInst();
		RenderState::Ref().SetTopology(PrimitiveTopology::TOPOLOGY_TRIANGLE);
		RenderState::Ref().SetViewport({ 0.0f, 0.0f, 
			static_cast<pvfloat>(conf.WindowWidth),
			static_cast<pvfloat>(conf.WindowHeight), 
			0.0f, 1.0f 
		});

		Renderer2D::CreateInst();

		VirtualMachine::Ref().DoString("print(Window.Title)");
	}

	Application::~Application() {
		Renderer2D::DestroyInst();
		GraphicsContext::DestroyInst();
		Window::DestroyInst();
		VirtualMachine::DestroyInst();

		PV_PROFILE_END_SESSION();
	}

	void Application::Run() {
		while (m_IsWindowOpen) {
			Timer::GetTime();
			PV_PROFILE_FUNCTION();

			GraphicsContext::Ref().BeginFrame();

			for (auto & layer : m_LayerStack) {
				layer->OnUpdate(0.0f);
			}

			Renderer2D::Ref().Render();

			GraphicsContext::Ref().EndFrame();

			Window::Ref().Update();

			//LOG_TRACE("Draw Calls : %d", NUMBER_OF_DRAW_CALLS);
			NUMBER_OF_DRAW_CALLS = 0;
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