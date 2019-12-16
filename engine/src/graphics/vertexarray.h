#pragma once

#include "../common/handledobject.h"
#include "../common/types.h"

#include "graphicscontext.h"
#include "buffer.h"
#include "shader.h"

namespace prev {

	class VertexArray : public HandledObject<VertexArray> {
		friend class StrongHandle<VertexArray>;
	protected:
		VertexArray() {}
		virtual ~VertexArray() {}
	public:
		virtual void Bind() = 0;
		virtual void UnBind() = 0;
		virtual void AddVertexBuffer(const StrongHandle<VertexBuffer> vertexBuffer) = 0;
		virtual void Draw(pvuint numElements) = 0;
	public:
		DECLARE_OBJECT(VertexArray, StrongHandle<VertexShader> vShader); // Vertex Shader Required By DirectX
	};

}