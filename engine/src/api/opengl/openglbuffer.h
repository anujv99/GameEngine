#pragma once

#ifdef ENGINE_OPENGL

#include "graphics/buffer.h"

#include <glad/glad.h>

namespace prev{

	GLenum GetOpenGLType(BufferUsage usage);

	// ------------- VERTEX BUFFER -------------

	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(const void * data, pvsizet size, pvuint stride, BufferUsage usage);
		~OpenGLVertexBuffer();
	public:
		virtual void Bind() const override;
		virtual void UnBind() const override;
		virtual void SubData(const void * data, pvsizet size, pvsizet offset) override;
		virtual void * Map() override;
		virtual void UnMap() override;
		virtual void SetBufferLayout(StrongHandle<BufferLayout> layout) override;
		virtual const StrongHandle<BufferLayout> GetLayout() const override { return m_Layout; }
		virtual pvuint GetStride() const override { return m_StrideBytes; };
	private:
		StrongHandle<BufferLayout> m_Layout;
		GLuint m_ID;
		pvsizet m_Size;
		pvuint m_StrideBytes;
		BufferUsage m_Usage;
		pvbool m_IsMapped;
	};

	// ------------- UNIFORM BUFFER -------------

	class OpenGLUniformBuffer : public UniformBuffer {
	public:
		OpenGLUniformBuffer(const void * data, pvsizet size, BufferUsage usage);
		~OpenGLUniformBuffer();
	public:
		virtual void Bind(pvuint block, ShaderType type) const override;
		virtual void UnBind() const override;
		virtual void SubData(const void * data, pvsizet size, pvsizet offset) override;
	private:
		GLuint m_ID;
		pvsizet m_Size;
	};
}

#endif