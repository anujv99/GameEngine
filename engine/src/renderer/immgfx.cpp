#include "immgfx.h"

namespace prev {
	
	#include "immgfxshaders.inl"

	static constexpr const size_t MAX_VERTICES = 1024;

	ImmGFX::ImmGFX() : Batcher(MAX_VERTICES), m_CurrentBatch(nullptr) {
		m_Layout = BufferLayout::Create();
		m_Layout->BeginEntries();
		m_Layout->AddEntry(DataType::Float2, offsetof(prev::Vertex, Position), "POSITION", false);
		m_Layout->AddEntry(DataType::UByte4, offsetof(prev::Vertex, Color), "COLOR", true);
		m_Layout->EndEntries();

		StrongHandle<FragmentShader> fShader;
		StrongHandle<VertexShader> vShader;

		if (GraphicsContext::Ref().GetAPI() == GraphicsAPI::API_DIRECTX) {
			vShader = VertexShader::Create(DX_V_SHADER);
			fShader = FragmentShader::Create(DX_F_SHADER);
		} else {
			vShader = VertexShader::Create(GL_V_SHADER);
			fShader = FragmentShader::Create(GL_F_SHADER);
		}

		m_VShader = vShader;
		
		m_Shader = ShaderProgram::Create(vShader, fShader);

	}

	ImmGFX::~ImmGFX() {}

	void ImmGFX::Render() {
		m_Shader->Bind();
		for (auto & batch : m_Batches) {
			if (batch.second->VBOIndex <= 0) continue;

			RenderState::Ref().SetTopology(batch.first);

			batch.second->VBO->UnMap();

			batch.second->VAO->Bind();
			batch.second->VAO->Draw((unsigned int)(batch.second->VBOIndex));

			batch.second->Data = reinterpret_cast<prev::Vertex *>(batch.second->VBO->Map());
			batch.second->VBOIndex = 0;
		}
	}

	void ImmGFX::PolygonBegin(PrimitiveTopology topology) {
		ASSERTM(m_CurrentBatch == nullptr, "Forgot to call PolygonEnd");
		m_CurrentBatch = GetBatch(topology);
	}

	void ImmGFX::PolygonEnd() {
		ASSERTM(m_CurrentBatch != nullptr, "Forgot to call PolygonBegin");
		m_CurrentBatch = nullptr;
	}

	void ImmGFX::Vertex(const Vec2 & pos) {
		m_CurrentBatch->Data[m_CurrentBatch->VBOIndex++] = { pos, m_CurrentColor };
	}

	StrongHandle<ImmGFX::Batch> ImmGFX::GetBatch(PrimitiveTopology topology) {
		auto it = m_Batches.find(topology);

		StrongHandle<Batch> batch;

		if (it == m_Batches.end()) {
			batch = new Batch(MAX_VERTICES * sizeof(prev::Vertex), m_Layout, m_VShader);
			m_Batches[topology] = batch;
		} else {
			batch = it->second;
		}

		return batch;
	}

	ImmGFX::Batch::Batch(size_t bufferSize, StrongHandle<BufferLayout> layout, StrongHandle<VertexShader> vs) :
		VBOIndex(0), Data(nullptr) {
		ASSERT(bufferSize > 0);
		VBO = VertexBuffer::Create(nullptr, bufferSize, sizeof(prev::Vertex), BufferUsage::USAGE_DYNAMIC);
		VBO->SetBufferLayout(layout);

		VAO = VertexArray::Create(vs);
		VAO->AddVertexBuffer(VBO);

		Data = reinterpret_cast<prev::Vertex *>(VBO->Map());
	}

}