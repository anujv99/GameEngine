#include "renderer2d.h"

#include "graphics/renderstate.h"
#include "math/mvpstack.h"

namespace prev {

	#include "batcherShaders.inl"
	#include "batcherRendererShaders.inl"

	Renderer2D::Renderer2D() : Batcher(MAX_VERTICES) {
		CreateBatcher();
		CreateRenderer();

		m_MVPBuffer = UniformBuffer::Create(static_cast<const void *>(Mat4().GetFloatPtr()), sizeof(Mat4), BufferUsage::USAGE_DYNAMIC);
		m_MVPBuffer->Bind(m_BatcherShader->GetUniformLocation("MVP", ShaderType::VERTEX_SHADER), ShaderType::VERTEX_SHADER);
	}

	Renderer2D::~Renderer2D() {}

	void Renderer2D::CreateBatcher() {
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

		m_BatcherShader = ShaderProgram::Create(vShader, fShader);

		m_VertexArray = VertexArray::Create(vShader);
		m_VertexArray->AddVertexBuffer(m_BatcherBuffer);
	}

	void Renderer2D::CreateRenderer() {
		m_RendererBuffer = VertexBuffer::Create(nullptr, MAX_VERTICES * sizeof(RendererVertex), sizeof(RendererVertex), BufferUsage::USAGE_DYNAMIC);

		StrongHandle<BufferLayout> layout = BufferLayout::Create();
		layout->BeginEntries();
		layout->AddEntry(DataType::Float2, offsetof(prev::Renderer2D::RendererVertex, Position), "POSITION", false);
		layout->AddEntry(DataType::Float2, offsetof(prev::Renderer2D::RendererVertex, TexCoord), "TEX_COORD", false);
		layout->AddEntry(DataType::Int, offsetof(prev::Renderer2D::RendererVertex, TexIndex), "TEX_INDEX", false);
		layout->EndEntries();

		m_RendererBuffer->SetBufferLayout(layout);

		StrongHandle<VertexShader> vShader;
		StrongHandle<FragmentShader> fShader;

		if (GraphicsContext::GetAPI() == GraphicsAPI::API_DIRECTX) {
			vShader = VertexShader::Create(DX_V_RENDERER_SHADER);
			fShader = FragmentShader::Create(DX_F_RENDERER_SHADER);
		} else {
			vShader = VertexShader::Create(GL_V_RENDERER_SHADER);
			fShader = FragmentShader::Create(GL_F_RENDERER_SHADER);
		}

		m_RendererShader = ShaderProgram::Create(vShader, fShader);

		m_RendererVertexArray = VertexArray::Create(vShader);
		m_RendererVertexArray->AddVertexBuffer(m_RendererBuffer);

		m_Vertices.resize(MAX_VERTICES);
		m_Textures.reserve(MAX_TEXTURES);
	}

	void Renderer2D::Render() {

		m_RendererBuffer->SubData(m_Vertices.data(), m_VertexIndex * sizeof(m_Vertices[0]), 0);

		for (pvsizet i = 0; i < m_Textures.size(); i++) {
			m_Textures[i]->Bind(static_cast<pvuint>(i));
		}

		RenderState::Ref().SetTopology(PrimitiveTopology::TOPOLOGY_TRIANGLE);

		m_RendererVertexArray->Bind();
		m_RendererShader->Bind();
		m_RendererVertexArray->Draw(m_VertexIndex);

		m_VertexIndex = 0;
		m_Textures.clear();
		m_Textures.reserve(MAX_TEXTURES);
	}

	pvint Renderer2D::SubmitTexture(StrongHandle<Texture2D> texture) {
		if (texture == nullptr) return -1;
		if (m_Textures.size() > MAX_TEXTURES) {
			EndScene();
		}
		for (pvsizet i = 0; i < m_Textures.size(); i++) {
			if (m_Textures[i] == texture) {
				return i;
			}
		}
		m_Textures.push_back(texture);
		return (m_Textures.size() - 1);
	}

	void Renderer2D::BeginScene(const StrongHandle<OrthoCamera> & cam) {
		if (MVPStack::Ref().IsChanged()) {
			m_MVPBuffer->SubData(static_cast<const void *>(MVPStack::Ref().GetMVP().GetFloatPtr()), sizeof(Mat4), 0);
		}
	}

	void Renderer2D::EndScene() {
		PV_PROFILE_FUNCTION();
		Render();

		m_VertexArray->Bind();
		m_BatcherShader->Bind();
		FlushAll();
	}

	void Renderer2D::DrawSprite(Vec2 pos, Vec2 size, StrongHandle<Texture2D> texture) {
		PV_PROFILE_FUNCTION();
		DrawSprite(pos, size, Vec2(0.0f), Vec2(1.0f), texture);
	}

	void Renderer2D::DrawSprite(Vec2 pos, Vec2 size, Vec2 texCoordTopLeft, Vec2 texCoordBottomRight, StrongHandle<Texture2D> texture) {
		PV_PROFILE_FUNCTION();
		pvint texIndex = SubmitTexture(texture);

		m_Vertices[m_VertexIndex++] = { pos + Vec2(-size.x, size.y) / 2.0f, texCoordTopLeft, texIndex };
		m_Vertices[m_VertexIndex++] = { pos + Vec2(size.x, size.y) / 2.0f, Vec2(texCoordBottomRight.x, texCoordTopLeft.y), texIndex };
		m_Vertices[m_VertexIndex++] = { pos + Vec2(size.x, -size.y) / 2.0f, texCoordBottomRight, texIndex };
		m_Vertices[m_VertexIndex++] = { pos + Vec2(-size.x, size.y) / 2.0f, texCoordTopLeft, texIndex };
		m_Vertices[m_VertexIndex++] = { pos + Vec2(size.x, -size.y) / 2.0f, texCoordBottomRight, texIndex };
		m_Vertices[m_VertexIndex++] = { pos + Vec2(-size.x, -size.y) / 2.0f, Vec2(texCoordTopLeft.x, texCoordBottomRight.y), texIndex };
	}

	void Renderer2D::DrawText(StrongHandle<Font> & font, const Label & label) {
		if (font == nullptr) {
			LOG_ERROR("Invalid font");
			return;
		}
		Vec2 cursorPos = label.Pos;
		const Vec2 size = font->GetSize(label.Text) * label.Scale;

		for (pvsizet i = 0; i < label.Text.size(); i++) {
			pvuchar c = label.Text[i];
			const Font::Character & character = font->GetCharacter(c);
			Vec2 topLeft = cursorPos + (Vec2(character.Size.x, -character.Size.y) * label.Scale) / 2.0f;
			topLeft += character.Offset * label.Scale;
			switch (label.Align) {
			case Label::Alignment::CENTER:
				topLeft -= size / 2.0f; break;
			case Label::Alignment::LEFT:
				topLeft -= Vec2(size.x, size.y / 2.0f); break;
			case Label::Alignment::RIGHT:
				topLeft -= Vec2(0.0f, size.y / 2.0f); break;
			}
			DrawSprite(topLeft, character.Size * label.Scale, character.TexCoordTopLeft,
				character.TexCoordBottomRight, font->GetTexture());
			cursorPos.x += character.XAdvance * label.Scale.x;
		}
	}

	void Renderer2D::Flush(PrimitiveTopology prim, const StrongHandle<Batch> batch) {
		PV_PROFILE_FUNCTION();

		RenderState::Ref().SetTopology(prim);
		m_BatcherBuffer->SubData(batch->GetData(), batch->GetDataSize(), 0);
		m_VertexArray->Draw(batch->GetNumElements());
	}

}