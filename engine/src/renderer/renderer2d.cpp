#include "renderer2d.h"

#include "graphics/renderstate.h"

namespace prev {

	static constexpr pvchar DX_V_SHADER[] = R"(
		struct VSOut {
			float4 oPos : SV_POSITION;
			float4 oColor : OUT_COLOR;
		};

		VSOut main(float2 iPos : POSITION, float4 iColor : COLOR) {
			VSOut vso;
			vso.oPos = float4(iPos.x, iPos.y, 0.0f, 1.0f);
			vso.oColor = iColor;
			return vso;
		}
	)";

	static constexpr pvchar DX_F_SHADER[] = R"(
		struct VSIn {
			float4 iPos : SV_POSITION;
			float4 iColor : OUT_COLOR;
		};

		float4 main(VSIn vsi) : SV_TARGET {
			return vsi.iColor;
		}
	)";

	static constexpr pvchar GL_V_SHADER[] = R"(
		#version 450 core

		layout (location = 0) in vec2 iPos;
		layout (location = 1) in vec4 iColor;

		out vec4 passColor;

		void main() {
			passColor = iColor;
			gl_Position = vec4(iPos.x, iPos.y, 0.0f, 1.0f);
		}
	)";

	static constexpr pvchar GL_F_SHADER[] = R"(
		#version 450 core

		out vec4 FragColor;

		in vec4 passColor;

		void main() {
			FragColor = passColor;
		}
	)";

	Renderer2D::Renderer2D() : Batcher(MAX_VERTICES) {
		m_BatcherBuffer = VertexBuffer::Create(nullptr, MAX_VERTICES * sizeof(prev::Vertex), sizeof(prev::Vertex), BufferUsage::USAGE_DYNAMIC);

		StrongHandle<BufferLayout> layout = BufferLayout::Create();
		layout->BeginEntries();
		layout->AddEntry(DataType::Float2, offsetof(prev::Vertex, Position), "POSITION", false);
		layout->AddEntry(DataType::UByte4, offsetof(prev::Vertex, Color), "COLOR", true);
		layout->EndEntries();

		m_BatcherBuffer->SetBufferLayout(layout);

		StrongHandle<VertexShader> vShader;
		StrongHandle<FragmentShader> fShader;

		if (GraphicsContext::GetAPI() == GraphicsAPI::API_DIRECTX) {
			vShader = VertexShader::Create(DX_V_SHADER);
			fShader = FragmentShader::Create(DX_F_SHADER);
		} else {
			vShader = VertexShader::Create(GL_V_SHADER);
			fShader = FragmentShader::Create(GL_F_SHADER);
		}

		m_Shader = ShaderProgram::Create(vShader, fShader);

		m_VertexArray = VertexArray::Create(vShader);
		m_VertexArray->AddVertexBuffer(m_BatcherBuffer);
	}

	Renderer2D::~Renderer2D() {}

	void Renderer2D::Render() {
		PV_PROFILE_FUNCTION();

		FlushAll();
	}

	void Renderer2D::Flush(PrimitiveTopology prim, const StrongHandle<Batch> batch) {
		PV_PROFILE_FUNCTION();

		RenderState::Ref().SetTopology(prim);
		m_BatcherBuffer->SubData(batch->GetData(), batch->GetDataSize(), 0);

		m_VertexArray->Bind();
		m_Shader->Bind();
		m_VertexArray->Draw(batch->GetNumElements());
	}

}