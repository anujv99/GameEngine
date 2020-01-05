#include "application.h"

#include "core/window.h"
#include "core/timer.h"

#include "graphics/graphicscontext.h"
#include "graphics/renderstate.h"

#include "graphics/buffer.h"
#include "graphics/shader.h"
#include "graphics/bufferlayout.h"
#include "graphics/vertexarray.h"
#include "graphics/texture.h"
#include "graphics/font.h"

#include "imgui/imgui.h"
#include "imgui/imguimanager.h"

#include "renderer/renderer2d.h"

#include "virtualmachine/virtualmachine.h"

#include "math/mvpstack.h"

static prev::MemoryLeakDetector s_LeakDetector;

namespace prev {
	
	pvuint NUMBER_OF_DRAW_CALLS = 0u;

	StrongHandle<Font> fnt;

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

		pvfloat aspect = static_cast<pvfloat>(conf.WindowWidth) / static_cast<pvfloat>(conf.WindowHeight);

		m_Camera = new OrthoCamera(-aspect, aspect, 1, -1);

		ImGuiManager::CreateInst();

		MVPStack::CreateInst();

		BlendFunction func;
		func.SrcColor			= BlendColorOption::BLEND_SRC_ALPHA;
		func.DestColor			= BlendColorOption::BLEND_INV_SRC_ALPHA;
		func.ColorOperation		= BlendOperation::BLEND_OP_ADD;

		func.SrcAlpha			= BlendAlphaOption::BLEND_ONE;
		func.DestAlpha			= BlendAlphaOption::BLEND_ONE;
		func.AlphaOperation		= BlendOperation::BLEND_OP_ADD;
		RenderState::Ref().SetBlendFunction(func);
	}

	Application::~Application() {
		MVPStack::DestroyInst();
		ImGuiManager::DestroyInst();
		Renderer2D::DestroyInst();
		RenderState::DestroyInst();
		GraphicsContext::DestroyInst();
		Window::DestroyInst();
		VirtualMachine::DestroyInst();

		PV_PROFILE_END_SESSION();
	}

	void Application::Run() {
		while (m_IsWindowOpen) {
			PV_PROFILE_FUNCTION();

			PreUpdate();

			Render();
			GUI();
			
			PostUpdate();

			///LOG_TRACE("Draw Calls : %d", NUMBER_OF_DRAW_CALLS);
			NUMBER_OF_DRAW_CALLS = 0;
		}
	}

	void Application::PreUpdate() {
		GraphicsContext::Ref().BeginFrame();
	}

	void Application::Render() {
		
		MVPStack::Ref().Projection().Push(m_Camera->GetProjectionView());
		Renderer2D::Ref().BeginScene(m_Camera);

		for (auto & layer : m_LayerStack) {
			layer->OnUpdate(0.0f);
		}

		Renderer2D::Ref().EndScene();
		MVPStack::Ref().Projection().Pop();
	}

	void Application::GUI() {
		StrongHandle<OrthoCamera> cam = new OrthoCamera(0, Window::Ref().GetWidth(), Window::Ref().GetHeight(), 0);
		MVPStack::Ref().Projection().Push(cam->GetProjectionView());
		Renderer2D::Ref().BeginScene(cam);

		ImGuiManager::Ref().DetectConsumeInputs();
		ImGuiManager::Ref().PreUpdate();

		ImGui::Begin("LOL", Vec2i(100));
		ImGui::End();

		ImGuiManager::Ref().PostUpdate();

		Renderer2D::Ref().EndScene();
		MVPStack::Ref().Projection().Pop();
	}

	void Application::PostUpdate() {
		GraphicsContext::Ref().EndFrame();
		Input::Update();
		Window::Ref().Update();
	}

	void Application::OnEvent(Event & e) {
		EventDispatcher dispatcher(e);
		
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));

		Input::OnEvent(e);

		for (auto & layer : m_LayerStack) {
			layer->OnEvent(e);
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent & e) {
		m_IsWindowOpen = false;
		return false;
	}

	bool Application::OnWindowResize(WindowResizeEvent & e) {
		RenderState::Ref().SetViewport({ 0.0f, 0.0f,
			static_cast<pvfloat>(e.GetWindowSizeX()),
			static_cast<pvfloat>(e.GetWindowSizeY()),
			0.0f, 1.0f
		});

		pvfloat aspect = static_cast<pvfloat>(e.GetWindowSizeX()) / static_cast<pvfloat>(e.GetWindowSizeY());

		if (aspect < 1.0f) {
			aspect = 1.0f / aspect;
			m_Camera = new OrthoCamera(-1.0f, 1.0f, aspect, -aspect);
		} else {
			m_Camera = new OrthoCamera(-aspect, aspect, 1.0f, -1.0f);
		}

		return false;
	}

}