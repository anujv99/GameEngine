
#ifndef __IMMGFX_H__
#define __IMMGFX_H__

#include "batcher.h"

#include "../common/singleton.h"
#include "../common/handledobject.h"

#include "../graphics/buffer.h"
#include "../graphics/bufferlayout.h"
#include "../graphics/shader.h"
#include "../graphics/vertexarray.h"

namespace prev {

	class ImmGFX : public Singleton<ImmGFX>, public Batcher {
		friend class Singleton<ImmGFX>;
	private:
		ImmGFX();
		~ImmGFX();
	public:
		void Render();
	private:
		virtual void PolygonBegin(PrimitiveTopology topology) override;
		virtual void PolygonEnd() override;
		virtual void Vertex(const Vec2 & pos) override;
	private:
		StrongHandle<ShaderProgram> m_Shader;
		StrongHandle<VertexShader> m_VShader;
		StrongHandle<BufferLayout> m_Layout;
		
		struct Batch : public HandledObject<Batch> {
			Batch(size_t bufferSize, StrongHandle<BufferLayout> layout, StrongHandle<VertexShader> vs);

			StrongHandle<VertexArray> VAO;
			StrongHandle<VertexBuffer> VBO;
			size_t VBOIndex;
			prev::Vertex * Data;
		};

		std::unordered_map<PrimitiveTopology, StrongHandle<Batch>> m_Batches;
	private:
		StrongHandle<Batch> GetBatch(PrimitiveTopology topology);
		StrongHandle<Batch> m_CurrentBatch;
	};

}

#endif //__IMMGFX_H__
