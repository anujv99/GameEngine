#pragma once

#include "../math/math.h"

#include "../common/singleton.h"

#include "../graphics/buffer.h"
#include "../graphics/shader.h"
#include "../graphics/vertexarray.h"
#include "../graphics/texture.h"
#include "../graphics/font.h"

#include "orthocamera.h"

namespace prev {

	class Renderer2D : public Singleton<Renderer2D> {
		friend class Singleton<Renderer2D>;
	private:
		Renderer2D();
		~Renderer2D();
	public:
		void BeginScene();
		void EndScene();

		void DrawSprite(Vec2 pos, Vec2 size, Vec4 color, StrongHandle<Texture2D> texture);
		void DrawSprite(Vec2 pos, Vec2 size, Vec2 texCoordTopLeft, Vec2 texCoordBottomRight, Vec4 color, StrongHandle<Texture2D> texture);
		void DrawText(StrongHandle<Font> & font, const Label & label);
	private:
		void CreateRenderer();

		pvint SubmitTexture(StrongHandle<Texture2D> texture);
	private:
		struct RendererVertex {
			Vec2 Position;
			Vec2 TexCoord;
			pvint TexIndex;
			pvubyte col[4];
		};
	private:
		StrongHandle<VertexBuffer> m_RendererBuffer;
		StrongHandle<VertexArray> m_RendererVertexArray;
		StrongHandle<ShaderProgram> m_RendererShader;
		StrongHandle<UniformBuffer> m_MVPBuffer;

		RendererVertex * m_Vertices = nullptr;
		std::vector<StrongHandle<Texture2D>> m_Textures;

		pvsizet m_VertexIndex = 0u;

		static constexpr const pvuint MAX_VERTICES = 512u * 6u;
		static constexpr const pvuint MAX_TEXTURES = 8u;
	};

}