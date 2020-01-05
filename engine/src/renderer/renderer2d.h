#pragma once

#include "../math/math.h"

#include "../common/singleton.h"

#include "../graphics/buffer.h"
#include "../graphics/shader.h"
#include "../graphics/vertexarray.h"
#include "../graphics/texture.h"
#include "../graphics/font.h"

#include "batcher.h"
#include "orthocamera.h"

namespace prev {

	class Renderer2D : public Singleton<Renderer2D>, public Batcher {
		friend class Singleton<Renderer2D>;
	private:
		Renderer2D();
		~Renderer2D();
	public:
		void BeginScene(const StrongHandle<OrthoCamera> & cam);
		void EndScene();

		void DrawSprite(Vec2 pos, Vec2 size, StrongHandle<Texture2D> texture);
		void DrawSprite(Vec2 pos, Vec2 size, Vec2 texCoordTopLeft, Vec2 texCoordBottomRight, StrongHandle<Texture2D> texture);
		void DrawText(StrongHandle<Font> & font, const Label & label);
	private:
		void CreateBatcher();
		void CreateRenderer();
		void Render();

		pvint SubmitTexture(StrongHandle<Texture2D> texture);

		virtual void Flush(PrimitiveTopology prim, const StrongHandle<Batch> batch) override;
	private:
		struct RendererVertex {
			Vec2 Position;
			Vec2 TexCoord;
			pvint TexIndex;
		};
	private:
		StrongHandle<VertexBuffer> m_BatcherBuffer, m_RendererBuffer;
		StrongHandle<VertexArray> m_VertexArray, m_RendererVertexArray;
		StrongHandle<ShaderProgram> m_BatcherShader, m_RendererShader;
		StrongHandle<UniformBuffer> m_MVPBuffer;

		std::vector<RendererVertex> m_Vertices;
		std::vector<StrongHandle<Texture2D>> m_Textures;

		pvsizet m_VertexIndex = 0u;

		static constexpr const pvuint MAX_VERTICES = 1024u;
		static constexpr const pvuint MAX_TEXTURES = 8u;
	};

}