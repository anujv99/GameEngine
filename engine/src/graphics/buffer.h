#pragma once

#include "../common/handledobject.h"
#include "../common/types.h"

#include "graphicscontext.h"
#include "bufferlayout.h"
#include "shader.h"

namespace prev {

	enum class BufferUsage {
		USAGE_STATIC,
		USAGE_DYNAMIC,
		USAGE_STREAM
	};

	// ------------- VERTEX BUFFER -------------

	class VertexBuffer : public HandledObject<VertexBuffer> {
		friend class StrongHandle<VertexBuffer>;
	protected:
		VertexBuffer() {}
		virtual ~VertexBuffer() {}
	public:
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		virtual void SubData(const void * data, pvsizet size, pvsizet offset) = 0;
		virtual void * Map() = 0;
		virtual void UnMap() = 0;
		virtual const StrongHandle<BufferLayout> GetLayout() const = 0;
		virtual void SetBufferLayout(StrongHandle<BufferLayout> layout) = 0;
		virtual pvuint GetStride() const = 0;

		DECLARE_OBJECT(VertexBuffer, const void * data, pvsizet size, pvuint stride, BufferUsage usage);
	};

	// ------------- UNIFORM BUFFER -------------

	class UniformBuffer : public HandledObject<UniformBuffer> {
		friend class StrongHandle<UniformBuffer>;
	protected:
		UniformBuffer() {}
		virtual ~UniformBuffer() {}
	public:
		virtual void Bind(pvuint block, ShaderType type) const = 0;
		virtual void UnBind() const = 0;
		virtual void SubData(const void * data, pvsizet size, pvsizet offset) = 0;

		DECLARE_OBJECT(UniformBuffer, const void * data, pvsizet size, BufferUsage usage);
	};

}