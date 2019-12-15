#pragma once

#ifdef ENGINE_OPENGL

#include <glad/glad.h>

#include "graphics/graphicscontext.h"
#include "graphics/vertexarray.h"
#include "graphics/buffer.h"

#include "common/stronghandle.h"

namespace prev {

	GLenum GetOpenGLType(DataType dataType);

	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();
	public:
		virtual void Bind() override;
		virtual void UnBind() override;
		virtual void AddVertexBuffer(const StrongHandle<VertexBuffer> vertexBuffer) override;
	private:
		GLuint m_ID;
		pvuint m_VBOIndex;
		std::vector<StrongHandle<VertexBuffer>> m_VertexBuffers;	
	};

}

#endif