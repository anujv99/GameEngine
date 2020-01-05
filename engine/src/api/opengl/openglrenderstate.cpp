#include "openglrenderstate.h"

#ifdef ENGINE_OPENGL

namespace prev {

	GLenum GetOpenGLType(PrimitiveTopology topology) {
		switch (topology) {
		case prev::PrimitiveTopology::TOPOLOGY_POINT:				return GL_POINTS;
		case prev::PrimitiveTopology::TOPOLOGY_LINE:				return GL_LINES;
		case prev::PrimitiveTopology::TOPOLOGY_LINE_ADJ:			return GL_LINES_ADJACENCY;
		case prev::PrimitiveTopology::TOPOLOGY_LINE_STRIP:			return GL_LINE_STRIP;
		case prev::PrimitiveTopology::TOPOLOGY_LINE_STRIP_ADJ:		return GL_LINE_STRIP_ADJACENCY;
		case prev::PrimitiveTopology::TOPOLOGY_TRIANGLE:			return GL_TRIANGLES;
		case prev::PrimitiveTopology::TOPOLOGY_TRIANGLE_ADJ:		return GL_TRIANGLES_ADJACENCY;
		case prev::PrimitiveTopology::TOPOLOGY_TRIANGLE_STRIP:		return GL_TRIANGLE_STRIP;
		case prev::PrimitiveTopology::TOPOLOGY_TRIANGLE_STRIP_ADJ:	return GL_TRIANGLE_STRIP_ADJACENCY;
		default:
		{
			ASSERTM(false, "[OpenGL] Invalid Primitive Topology");
		}
		}
	}

	GLenum GetOpenGLType(BlendColorOption option) {
		switch (option) {
		case prev::BlendColorOption::BLEND_ZERO:			return GL_ZERO;
		case prev::BlendColorOption::BLEND_ONE:				return GL_ONE;
		case prev::BlendColorOption::BLEND_SRC_COLOR:		return GL_SRC_COLOR;
		case prev::BlendColorOption::BLEND_INV_SRC_COLOR:	return GL_ONE_MINUS_SRC_COLOR;
		case prev::BlendColorOption::BLEND_DEST_COLOR:		return GL_DST_COLOR;
		case prev::BlendColorOption::BLEND_INV_DEST_COLOR:	return GL_ONE_MINUS_DST_COLOR;
		case prev::BlendColorOption::BLEND_SRC_ALPHA:		return GL_SRC_ALPHA;
		case prev::BlendColorOption::BLEND_INV_SRC_ALPHA:	return GL_ONE_MINUS_SRC_ALPHA;
		case prev::BlendColorOption::BLEND_DEST_ALPHA:		return GL_DST_ALPHA;
		case prev::BlendColorOption::BLEND_INV_DEST_ALPHA:	return GL_ONE_MINUS_DST_ALPHA;
		default:
		{
			ASSERTM(false, "[OpenGL] Invalid BlendColorOption");
		}
		}
	}

	GLenum GetOpenGLType(BlendAlphaOption option) {
		switch (option) {
		case prev::BlendAlphaOption::BLEND_ZERO:			return GL_ZERO;
		case prev::BlendAlphaOption::BLEND_ONE:				return GL_ONE;
		case prev::BlendAlphaOption::BLEND_SRC_ALPHA:		return GL_SRC_ALPHA;
		case prev::BlendAlphaOption::BLEND_INV_SRC_ALPHA:	return GL_ONE_MINUS_SRC_ALPHA;
		case prev::BlendAlphaOption::BLEND_DEST_ALPHA:		return GL_DST_ALPHA;
		case prev::BlendAlphaOption::BLEND_INV_DEST_ALPHA:	return GL_ONE_MINUS_DST_ALPHA;
		default:
		{
			ASSERTM(false, "[OpenGL] Invalid BlendAlphaOption");
		}
		}
	}

	GLenum GetOpenGLType(BlendOperation operation) {
		switch (operation) {
		case prev::BlendOperation::BLEND_OP_ADD:			return GL_FUNC_ADD;
		case prev::BlendOperation::BLEND_OP_SUBTRACT:		return GL_SUBTRACT;
		case prev::BlendOperation::BLEND_OP_REV_SUBTRACT:	return GL_FUNC_REVERSE_SUBTRACT;
		case prev::BlendOperation::BLEND_OP_MIN:			return GL_MIN;
		case prev::BlendOperation::BLEND_OP_MAX:			return GL_MAX;
		default:
		{
			ASSERTM(false, "[OpenGL] Invalid BlendOperation");
		}
		}
	}

	DEFINE_SINGLE_OBJECT_OPENGL(RenderState) {
		return dynamic_cast<RenderState *>(new OpenGLRenderState());
	}

	OpenGLRenderState::OpenGLRenderState() : m_Prim(), m_Viewport(), m_OpenGLPrim(GL_INVALID_ENUM) {}

	void OpenGLRenderState::SetTopology(PrimitiveTopology topology) {
		PV_PROFILE_FUNCTION();
		m_OpenGLPrim = GetOpenGLType(topology);
		m_Prim = topology;
	}

	void OpenGLRenderState::SetViewport(Viewport viewport) {
		PV_PROFILE_FUNCTION();
		glViewport(viewport.TopLeftX, viewport.TopLeftY, viewport.Width, viewport.Height);
		m_Viewport = viewport;
	}

	void OpenGLRenderState::SetBlendFunction(BlendFunction func) {
		PV_PROFILE_FUNCTION();

		if (!func.EnableBlending) {
			glDisable(GL_BLEND);
		} else {
			glEnable(GL_BLEND);
			glBlendFuncSeparate(
				GetOpenGLType(func.SrcColor), 
				GetOpenGLType(func.DestColor), 
				GetOpenGLType(func.SrcAlpha), 
				GetOpenGLType(func.DestAlpha));
			glBlendEquationSeparate(GetOpenGLType(func.ColorOperation), GetOpenGLType(func.AlphaOperation));
		}

		m_BlendFunc = func;
	}

	PrimitiveTopology OpenGLRenderState::GetTopology() const {
		return m_Prim;
	}

	Viewport OpenGLRenderState::GetViewport() const {
		return m_Viewport;
	}

	BlendFunction OpenGLRenderState::GetBlendFunction() const {
		return m_BlendFunc;
	}

}

#endif