#include "openglbuffer.h"

#ifdef ENGINE_OPENGL

namespace prev {

	// ------------- VERTEX BUFFER -------------

	DEFINE_OBJECT_OPENGL(VertexBuffer, const void * data, pvsizet size, pvuint stride, BufferUsage usage) {
		return dynamic_cast<VertexBuffer *>(new OpenGLVertexBuffer(data, size, stride, usage));
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(const void * data, pvsizet size, pvuint stride, BufferUsage usage) : 
		m_ID(0u), m_StrideBytes(stride), m_Layout(nullptr) , m_Size(size), m_Usage(usage), m_IsMapped(false) {
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
		ASSERT(size + offset <= m_Size);
		ASSERTM(m_Usage != BufferUsage::USAGE_STATIC, "[OpenGL] Cannot use SubData on buffer with Static usage");
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferSubData(GL_ARRAY_BUFFER, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(size), data);
	}

	void * OpenGLVertexBuffer::Map() {
		PV_PROFILE_FUNCTION();
		ASSERTM(m_Usage != BufferUsage::USAGE_STATIC, "[OpenGL] Cannot use SubData on buffer with Static usage");
		ASSERTM(m_IsMapped == false, "[OpenGL] Buffer already mapped. Use UnMap after calling Map");

		void * mb = glMapNamedBuffer(m_ID, GL_WRITE_ONLY);
		ASSERTM(mb, "[OpenGL] Failed to map buffer");
		m_IsMapped = true;
		return mb;
	}

	void OpenGLVertexBuffer::UnMap() {
		PV_PROFILE_FUNCTION();
		ASSERTM(m_IsMapped == true, "[OpenGL] Buffer not mapped. Use Map before calling UnMap");

		ASSERTM(glUnmapNamedBuffer(m_ID) == GL_TRUE, "[OpenGL] Failed to UnMap buffer");
		m_IsMapped = false;
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
		ASSERT(size + offset <= m_Size);
		glNamedBufferSubData(m_ID, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(size), data);
	}

}

#endif