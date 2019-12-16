#pragma once

#ifdef ENGINE_OPENGL

#include "graphics/buffer.h"

#include <glad/glad.h>

namespace prev{

	GLenum GetOpenGLType(BufferUsage usage);

	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(const void * data, pvsizet size, pvuint stride, BufferUsage usage);
		~OpenGLVertexBuffer();
	public:
		virtual void Bind() const override;
		virtual void UnBind() const override;
		virtual void SetBufferLayout(StrongHandle<BufferLayout> layout) override;
		virtual const StrongHandle<BufferLayout> GetLayout() const override { return m_Layout; }
		virtual pvuint GetStride() const override { return m_StrideBytes; };
	private:
		StrongHandle<BufferLayout> m_Layout;
		GLuint m_ID;
		pvuint m_StrideBytes;
	};

}

#endif