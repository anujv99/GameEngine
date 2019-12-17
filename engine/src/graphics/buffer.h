#pragma once

#include "../common/handledobject.h"
#include "../common/types.h"

#include "graphicscontext.h"
#include "bufferlayout.h"

namespace prev {

	enum class BufferUsage {
		USAGE_STATIC,
		USAGE_DYNAMIC,
		USAGE_STREAM
	};

	class VertexBuffer : public HandledObject<VertexBuffer> {
		friend class StrongHandle<VertexBuffer>;
	protected:
		VertexBuffer() {}
		virtual ~VertexBuffer() {}
	public:
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		virtual void SubData(const void * data, pvsizet size, pvsizet offset) = 0;
		virtual const StrongHandle<BufferLayout> GetLayout() const = 0;
		virtual void SetBufferLayout(StrongHandle<BufferLayout> layout) = 0;
		virtual pvuint GetStride() const = 0;

		DECLARE_OBJECT(VertexBuffer, const void * data, pvsizet size, pvuint stride, BufferUsage usage);
	};

}