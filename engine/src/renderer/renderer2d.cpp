#include "renderer2d.h"

#include "graphics/renderstate.h"
#include "math/mvpstack.h"

namespace prev {

	#include "batcherShaders.inl"
	#include "batcherRendererShaders.inl"

	Renderer2D::Renderer2D() {
		CreateRenderer();

		m_MVPBuffer = UniformBuffer::Create(static_cast<const void *>(Mat4().GetFloatPtr()), sizeof(Mat4), BufferUsage::USAGE_DYNAMIC);
		m_MVPBuffer->Bind(m_RendererShader->GetUniformLocation("MVP", ShaderType::VERTEX_SHADER), ShaderType::VERTEX_SHADER);
	}

	Renderer2D::~Renderer2D() {}

	void Renderer2D::CreateRenderer() {
		m_RendererBuffer = VertexBuffer::Create(nullptr, MAX_VERTICES * sizeof(RendererVertex), sizeof(RendererVertex), BufferUsage::USAGE_DYNAMIC);

		StrongHandle<BufferLayout> layout = BufferLayout::Create();
		layout->BeginEntries();
		layout->AddEntry(DataType::Float2, offsetof(prev::Renderer2D::RendererVertex, Position), "POSITION", false);
		layout->AddEntry(DataType::Float2, offsetof(prev::Renderer2D::RendererVertex, TexCoord), "TEX_COORD", false);
		layout->AddEntry(DataType::Int, offsetof(prev::Renderer2D::RendererVertex, TexIndex), "TEX_INDEX", false);
		layout->AddEntry(DataType::UByte4, offsetof(prev::Renderer2D::RendererVertex, col), "COLOR", true);
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

		m_Textures.reserve(MAX_TEXTURES);
	}

	pvint Renderer2D::SubmitTexture(StrongHandle<Texture2D> texture) {
		PV_PROFILE_FUNCTION();
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

	void Renderer2D::BeginScene() {
		PV_PROFILE_FUNCTION();
		if (MVPStack::Ref().IsChanged()) {
			m_MVPBuffer->SubData(static_cast<const void *>(MVPStack::Ref().GetMVP().GetFloatPtr()), sizeof(Mat4), 0);
		}
		m_Vertices = reinterpret_cast<RendererVertex *>(m_RendererBuffer->Map());
	}

	void Renderer2D::EndScene() {
		PV_PROFILE_FUNCTION();
		m_RendererBuffer->UnMap();
		if (m_VertexIndex <= 0) return;

		for (pvsizet i = 0; i < m_Textures.size(); i++) {
			m_Textures[i]->Bind(static_cast<pvuint>(i));
		}

		RenderState::Ref().SetTopology(PrimitiveTopology::TOPOLOGY_TRIANGLE);

		m_RendererVertexArray->Bind();
		m_RendererShader->Bind();
		m_RendererVertexArray->Draw(m_VertexIndex);

		m_VertexIndex = 0;
		m_Textures.resize(0);
		m_Textures.reserve(MAX_TEXTURES);

	}

	void Renderer2D::DrawSprite(Vec2 pos, Vec2 size, Vec4 color, StrongHandle<Texture2D> texture) {
		PV_PROFILE_FUNCTION();
		DrawSprite(pos, size, Vec2(0.0f), Vec2(1.0f), color, texture);
	}

	void Renderer2D::DrawSprite(Vec2 pos, Vec2 size,
		Vec2 texCoordTopLeft, Vec2 texCoordBottomRight, Vec4 color, StrongHandle<Texture2D> texture) {

		PV_PROFILE_FUNCTION();
		if (m_VertexIndex + 6 >= MAX_VERTICES) { EndScene(); BeginScene(); }

		pvint texIndex = SubmitTexture(texture);

		m_Vertices[m_VertexIndex++] = { pos + Vec2(-size.x, size.y) / 2.0f, texCoordTopLeft, texIndex,
			static_cast<pvubyte>(color.x * 255.0f),
			static_cast<pvubyte>(color.y * 255.0f),
			static_cast<pvubyte>(color.z * 255.0f),
			static_cast<pvubyte>(color.w * 255.0f)
		};

		m_Vertices[m_VertexIndex++] = { pos + Vec2(size.x, size.y) / 2.0f, Vec2(texCoordBottomRight.x, texCoordTopLeft.y), texIndex,
			static_cast<pvubyte>(color.x * 255.0f),
			static_cast<pvubyte>(color.y * 255.0f),
			static_cast<pvubyte>(color.z * 255.0f),
			static_cast<pvubyte>(color.w * 255.0f)
		};

		m_Vertices[m_VertexIndex++] = { pos + Vec2(size.x, -size.y) / 2.0f, texCoordBottomRight, texIndex,
			static_cast<pvubyte>(color.x * 255.0f),
			static_cast<pvubyte>(color.y * 255.0f),
			static_cast<pvubyte>(color.z * 255.0f),
			static_cast<pvubyte>(color.w * 255.0f)
		};

		m_Vertices[m_VertexIndex++] = { pos + Vec2(-size.x, size.y) / 2.0f, texCoordTopLeft, texIndex,
			static_cast<pvubyte>(color.x * 255.0f),
			static_cast<pvubyte>(color.y * 255.0f),
			static_cast<pvubyte>(color.z * 255.0f),
			static_cast<pvubyte>(color.w * 255.0f)
		};

		m_Vertices[m_VertexIndex++] = { pos + Vec2(size.x, -size.y) / 2.0f, texCoordBottomRight, texIndex,
			static_cast<pvubyte>(color.x * 255.0f),
			static_cast<pvubyte>(color.y * 255.0f),
			static_cast<pvubyte>(color.z * 255.0f),
			static_cast<pvubyte>(color.w * 255.0f)
		};

		m_Vertices[m_VertexIndex++] = { pos + Vec2(-size.x, -size.y) / 2.0f, Vec2(texCoordTopLeft.x, texCoordBottomRight.y), texIndex,
			static_cast<pvubyte>(color.x * 255.0f),
			static_cast<pvubyte>(color.y * 255.0f),
			static_cast<pvubyte>(color.z * 255.0f),
			static_cast<pvubyte>(color.w * 255.0f)
		};

	}

	void Renderer2D::DrawText(StrongHandle<Font> & font, const Label & label) {
		PV_PROFILE_FUNCTION();
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
				character.TexCoordBottomRight, label.Color, font->GetTexture());
			cursorPos.x += character.XAdvance * label.Scale.x;
		}
	}

}