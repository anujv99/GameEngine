#include "openglbuffer.h"

#ifdef ENGINE_OPENGL

namespace prev {

	// ------------- VERTEX BUFFER -------------

	DEFINE_OBJECT_OPENGL(VertexBuffer, const void * data, pvsizet size, pvuint stride, BufferUsage usage) {
		return dynamic_cast<VertexBuffer *>(new OpenGLVertexBuffer(data, size, stride, usage));
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(const void * data, pvsizet size, pvuint stride, BufferUsage usage) : 
		m_ID(0u), m_StrideBytes(stride), m_Layout(nullptr) , m_Size(size) {
		PV_PROFILE_FUNCTION();
		ASSERT(size > 0);
		glCreateBuffers(1, &m_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ARRAY_BUFFER, size, data, GetOpenGLType(usage));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer() {
		glDeleteBuffers(1, &m_ID);
	}

	void OpenGLVertexBuffer::Bind() const  {
		PV_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	}

	void OpenGLVertexBuffer::UnBind() const  {
		glBindBuffer(GL_ARRAY_BUFFER, 0u);
	}

	void OpenGLVertexBuffer::SubData(const void * data, pvsizet size, pvsizet offset) {
		PV_PROFILE_FUNCTION();
		ASSERT(data);
		ASSERT(size <= m_Size);
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferSubData(GL_ARRAY_BUFFER, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(size), data);
	}

	void OpenGLVertexBuffer::SetBufferLayout(StrongHandle<BufferLayout> layout) {
		if (layout == nullptr) {
			LOG_ERROR("Inavlid buffer layout");
			return;
		}
		m_Layout = layout;
	}

	GLenum GetOpenGLType(BufferUsage usage) {
		switch (usage) {
		case prev::BufferUsage::USAGE_STATIC:
			return GL_STATIC_DRAW;
		case prev::BufferUsage::USAGE_DYNAMIC:
			return GL_DYNAMIC_DRAW;
		case prev::BufferUsage::USAGE_STREAM:
			return GL_STREAM_DRAW;
		default:
			return GL_INVALID_ENUM;
		}
	}

	// ------------- UNIFORM BUFFER -------------

	DEFINE_OBJECT_OPENGL(UniformBuffer, const void * data, pvsizet size, BufferUsage usage) {
		return dynamic_cast<UniformBuffer *>(new OpenGLUniformBuffer(data, size, usage));
	}

	OpenGLUniformBuffer::OpenGLUniformBuffer(const void * data, pvsizet size, BufferUsage usage) :
		m_ID(0u), m_Size(0u) {
		PV_PROFILE_FUNCTION();
		ASSERT(size > 0);
		if (size % 16 != 0) {
			size += 16 - (size % 16);
		}

		m_Size = size;
		glCreateBuffers(1, &m_ID);
		glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
		glBufferData(GL_UNIFORM_BUFFER, size, data, GetOpenGLType(usage));
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer() {}

	void OpenGLUniformBuffer::Bind(pvuint block, ShaderType type) const {
		PV_PROFILE_FUNCTION();
		glBindBufferBase(GL_UNIFORM_BUFFER, static_cast<GLuint>(block), m_ID);
	}

	void OpenGLUniformBuffer::UnBind() const {}

	void OpenGLUniformBuffer::SubData(const void * data, pvsizet size, pvsizet offset) {
		PV_PROFILE_FUNCTION();
		ASSERT(data);
		ASSERT(size <= m_Size);
		glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
		glBufferSubData(GL_UNIFORM_BUFFER, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(size), data);
	}

}

#endif