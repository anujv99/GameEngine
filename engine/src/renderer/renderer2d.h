#pragma once

#include "../math/math.h"

#include "../common/singleton.h"

#include "../graphics/buffer.h"
#include "../graphics/shader.h"
#include "../graphics/vertexarray.h"

#include "batcher.h"

namespace prev {

	class Renderer2D : public Singleton<Renderer2D>, public Batcher {
		friend class Singleton<Renderer2D>;
	private:
		Renderer2D();
		~Renderer2D();
	public:
		void Render();
	private:
		virtual void Flush(PrimitiveTopology prim, const StrongHandle<Batch> batch) override;
	private:
		StrongHandle<VertexBuffer> m_BatcherBuffer;
		StrongHandle<VertexArray> m_VertexArray;
		StrongHandle<ShaderProgram> m_Shader;

		static constexpr pvuint MAX_VERTICES = 1024u;
	};

}