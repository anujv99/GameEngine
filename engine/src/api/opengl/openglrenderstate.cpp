#include "openglrenderstate.h"

#ifdef ENGINE_OPENGL

namespace prev {

	GLenum GetOpenGLType(PrimitiveTopology topology) {
		switch (topology) {
		case prev::PrimitiveTopology::TOPOLOGY_POINT:					return GL_POINTS;
		case prev::PrimitiveTopology::TOPOLOGY_LINE:					return GL_LINES;
		case prev::PrimitiveTopology::TOPOLOGY_LINE_ADJ:				return GL_LINES_ADJACENCY;
		case prev::PrimitiveTopology::TOPOLOGY_LINE_STRIP:				return GL_LINE_STRIP;
		case prev::PrimitiveTopology::TOPOLOGY_LINE_STRIP_ADJ:			return GL_LINE_STRIP_ADJACENCY;
		case prev::PrimitiveTopology::TOPOLOGY_TRIANGLE:				return GL_TRIANGLES;
		case prev::PrimitiveTopology::TOPOLOGY_TRIANGLE_ADJ:			return GL_TRIANGLES_ADJACENCY;
		case prev::PrimitiveTopology::TOPOLOGY_TRIANGLE_STRIP:			return GL_TRIANGLE_STRIP;
		case prev::PrimitiveTopology::TOPOLOGY_TRIANGLE_STRIP_ADJ:		return GL_TRIANGLE_STRIP_ADJACENCY;
		default:
		{
			ASSERTM(false, "Invalid Primitive Topology");
		}
		}
	}

	DEFINE_SINGLE_OBJECT_OPENGL(RenderState) {
		return dynamic_cast<RenderState *>(new OpenGLRenderState());
	}

	OpenGLRenderState::OpenGLRenderState() : m_Prim(), m_Viewport(), m_OpenGLPrim(GL_INVALID_ENUM) {}

	void OpenGLRenderState::SetTopology(PrimitiveTopology topology) {
		m_OpenGLPrim = GetOpenGLType(topology);
		m_Prim = topology;
	}

	void OpenGLRenderState::SetViewport(Viewport viewport) {
		glViewport(viewport.TopLeftX, viewport.TopLeftY, viewport.Width, viewport.Height);
		m_Viewport = viewport;
	}

	PrimitiveTopology OpenGLRenderState::GetTopology() const {
		return m_Prim;
	}

	Viewport OpenGLRenderState::GetViewport() const {
		return m_Viewport;
	}

}

#endif