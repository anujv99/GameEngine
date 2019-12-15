#include "application.h"

#include "core/window.h"
#include "graphics/graphicscontext.h"

#include "graphics/buffer.h"
#include "graphics/shader.h"
#include "graphics/bufferlayout.h"
#include "graphics/vertexarray.h"

#include "debug/instrumentor.h"

#include "api/directx/directxhelper.h"

namespace prev {

	StrongHandle<ShaderProgram> s;
	StrongHandle<VertexArray> a;

	Application::Application() : m_IsWindowOpen(true) {
		PV_PROFILE_BEGIN_SESSION("EveryThing", "gg.json");

		PV_PROFILE_FUNCTION();

		GraphicsContext::SetGraphicsAPI(GraphicsAPI::API_DIRECTX);

		Window::CreateInst();
		Window::Ref().Create(WindowProps(1280, 720, "Game Engine Window"));
		Window::Ref().SetEventFunction(BIND_EVENT_FN(Application::OnEvent));

		GraphicsContext::CreateInst();
		GraphicsContext::Ref().CreateContext(Window::Ref().GetRawWindowPtr());

		float arr[] = {
			 0.0f,  0.5f,
			 0.5f, -0.5f,
			-0.5f, -0.5f,
		};

		auto b = VertexBuffer::Create(arr, sizeof(arr), 2 * sizeof(float), BufferUsage::USAGE_STATIC);

		auto l = BufferLayout::Create();
		l->BeginEntries(2 * sizeof(float));
		l->AddEntry(DataType::Float2, 0u, "POSITION");
		l->EndEntries();

		b->SetBufferLayout(l);

		StrongHandle<VertexShader> v;
		StrongHandle<FragmentShader> f;

		if (GraphicsContext::GetAPI() == GraphicsAPI::API_OPENGL) {

			v = VertexShader::Create(
				R"(
					#version 450 core
					
					layout (location = 0) in vec2 aPos;

					void main() {
						gl_Position = vec4(aPos, 0.0f, 1.0f);
					}
				)"
			);

			f = FragmentShader::Create(
				R"(
					#version 450 core
					
					out vec4 FragColor;

					void main() {
						FragColor = vec4(1.0f);
					}
				)"
			);

		}  else {
			v = VertexShader::Create(
				R"(
					float4 main(float2 pos : POSITION) : SV_POSITION {
						return float4(pos.x, pos.y, 0.0f, 1.0f);
					}
				)"
			);

			f = FragmentShader::Create(
				R"(
					float4 main() : SV_TARGET {
						return float4(1.0f, 1.0f, 1.0f, 1.0f);
					}
				)"
			);
		}

		s = ShaderProgram::Create(v, f);

		a = VertexArray::Create(v);
		a->AddVertexBuffer(b);

		GetDeviceContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		D3D11_VIEWPORT vp;
		vp.TopLeftX = -1.0f;
		vp.TopLeftY = 1.0f;
		vp.Width = 2.0f;
		vp.Height = 2.0f;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;

		GetDeviceContext()->RSSetViewports(1, &vp);

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

			GetDeviceContext()->Draw(3, 0);

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