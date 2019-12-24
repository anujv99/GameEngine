#include "openglvertexarray.h"

#ifdef ENGINE_OPENGL

#include "openglrenderstate.h"

namespace prev {
	
	extern pvuint NUMBER_OF_DRAW_CALLS;

	GLenum GetOpenGLType(DataType dataType) {
		switch (dataType) {
		case prev::DataType::Int:		return GL_INT;
		case prev::DataType::Int2:		return GL_INT;
		case prev::DataType::Int3:		return GL_INT;
		case prev::DataType::Int4:		return GL_INT;
		case prev::DataType::UInt:		return GL_UNSIGNED_INT;
		case prev::DataType::UInt2:		return GL_UNSIGNED_INT;
		case prev::DataType::UInt3:		return GL_UNSIGNED_INT;
		case prev::DataType::UInt4:		return GL_UNSIGNED_INT;
		case prev::DataType::Float:		return GL_FLOAT;
		case prev::DataType::Float2:	return GL_FLOAT;
		case prev::DataType::Float3:	return GL_FLOAT;
		case prev::DataType::Float4:	return GL_FLOAT;
		case prev::DataType::Bool:		return GL_BOOL;
		case prev::DataType::UByte:		return GL_UNSIGNED_BYTE;
		case prev::DataType::UByte2:	return GL_UNSIGNED_BYTE;
		case prev::DataType::UByte4:	return GL_UNSIGNED_BYTE;
		case prev::DataType::Byte:		return GL_BYTE;
		case prev::DataType::Byte2:		return GL_BYTE;
		case prev::DataType::Byte4:		return GL_BYTE;
		}

		ASSERTM(false, "Inavlid data type");
		return GL_INVALID_ENUM;
	}

	DEFINE_OBJECT_OPENGL(VertexArray, StrongHandle<VertexShader> vShader) {
		return dynamic_cast<VertexArray *>(new OpenGLVertexArray());
	}

	OpenGLVertexArray::OpenGLVertexArray() : m_ID(0u), m_VBOIndex(0u), m_VertexBuffers() {
		PV_PROFILE_FUNCTION();
		glCreateVertexArrays(1, &m_ID);
	}

	OpenGLVertexArray::~OpenGLVertexArray() {
		glDeleteVertexArrays(1, &m_ID);
	}

	void OpenGLVertexArray::Bind() {
		PV_PROFILE_FUNCTION();
		glBindVertexArray(m_ID);
	}

	void OpenGLVertexArray::UnBind() {
		glBindVertexArray(0u);
	}

	void OpenGLVertexArray::AddVertexBuffer(const StrongHandle<VertexBuffer> vertexBuffer) {
		PV_PROFILE_FUNCTION();
		ASSERTM(vertexBuffer->GetLayout() != nullptr && vertexBuffer->GetLayout()->GetNumEntries() > 0, "Vertex Buffer has inavlid layout");

		Bind();
		vertexBuffer->Bind();

		const StrongHandle<BufferLayout> & layout = vertexBuffer->GetLayout();
		for (const auto & entry : *layout) {
			glEnableVertexAttribArray(m_VBOIndex);
			glVertexAttribPointer(
				m_VBOIndex,
				entry.GetNumElements(),
				GetOpenGLType(entry.Type),
				entry.Normalize ? GL_TRUE : GL_FALSE,
				vertexBuffer->GetStride(),
				reinterpret_cast<const void*>(entry.OffsetBytes)
			);
			m_VBOIndex++;
		}
		
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::Draw(pvuint numElements) {
		PV_PROFILE_FUNCTION();
		glDrawArrays(dynamic_cast<OpenGLRenderState *>(OpenGLRenderState::Get())->GetOpenGLTopology(), 0, numElements);
		NUMBER_OF_DRAW_CALLS++;
	}

}

#endif