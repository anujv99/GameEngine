#include "application.h"

#include "core/window.h"

#include "graphics/graphicscontext.h"
#include "graphics/renderstate.h"

#include "graphics/buffer.h"
#include "graphics/shader.h"
#include "graphics/bufferlayout.h"
#include "graphics/vertexarray.h"

namespace prev {

	StrongHandle<ShaderProgram> s;
	StrongHandle<VertexArray> a;

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

		float vertex[] = {
			 0.0f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
		};

		auto bv = VertexBuffer::Create(vertex, sizeof(vertex), 6 * sizeof(float), BufferUsage::USAGE_STATIC);

		auto lv = BufferLayout::Create();
		lv->BeginEntries();
		lv->AddEntry(DataType::Float2, 0u, "POSITION");
		lv->AddEntry(DataType::Float4, 2u * sizeof(float), "INCOLOR");
		lv->EndEntries();

		bv->SetBufferLayout(lv);

		StrongHandle<VertexShader> v;
		StrongHandle<FragmentShader> f;

		if (GraphicsContext::GetAPI() == GraphicsAPI::API_OPENGL) {

			v = VertexShader::Create(
				R"(
					#version 450 core
					
					layout (location = 0) in vec2 aPos;
					layout (location = 1) in vec4 aColor;
					
					out vec4 aColorPass;

					void main() {
						aColorPass = aColor;
						gl_Position = vec4(aPos, 0.0f, 1.0f);
					}
				)"
			);

			f = FragmentShader::Create(
				R"(
					#version 450 core
					
					out vec4 FragColor;

					in vec4 aColorPass;

					void main() {
						FragColor = aColorPass;
					}
				)"
			);

		}  else {
			v = VertexShader::Create(
				R"(
					struct VSOut {
						float4 aPos : SV_POSITION;
						float4 aColor : COLOR;
					};

					VSOut main(float2 pos : POSITION, float4 color : INCOLOR) {
						VSOut vso;
						vso.aPos = float4(pos.x, pos.y, 0.0f, 1.0f);
						vso.aColor = color;
						return vso;
					}
				)"
			);

			f = FragmentShader::Create(
				R"(
					struct VSIn {
						float4 aPos : SV_POSITION;
						float4 aColor : COLOR;
					};
					
					float4 main(VSIn vsi) : SV_TARGET {
						return float4(vsi.aColor);
					}
				)"
			);

		}

		s = ShaderProgram::Create(v, f);

		a = VertexArray::Create(v);
		a->AddVertexBuffer(bv);
	}

	Application::~Application() {
		GraphicsContext::DestroyInst();
		Window::DestroyInst();

		PV_PROFILE_END_SESSION();
	}

	void Application::Run() {
		while (m_IsWindowOpen) {

			PV_PROFILE_FUNCTION();

			GraphicsContext::Ref().BeginFrame();

			a->Bind();
			s->Bind();

			a->Draw(3);

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