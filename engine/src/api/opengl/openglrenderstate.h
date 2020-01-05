#pragma once

#ifdef ENGINE_OPENGL

#include "graphics/renderstate.h"

#include <glad/glad.h>

namespace prev {

	GLenum GetOpenGLType(PrimitiveTopology topology);

	GLenum GetOpenGLType(BlendColorOption option);
	GLenum GetOpenGLType(BlendAlphaOption option);
	GLenum GetOpenGLType(BlendOperation operation);

	class OpenGLRenderState : public RenderState {
	public:
		OpenGLRenderState();
		~OpenGLRenderState() {}
	public:
		virtual void SetTopology(PrimitiveTopology topology) override;
		virtual void SetViewport(Viewport viewport) override;
		virtual void SetBlendFunction(BlendFunction func) override;

		virtual PrimitiveTopology GetTopology() const override;
		virtual Viewport GetViewport() const override;
		virtual BlendFunction GetBlendFunction() const override;

		inline GLenum GetOpenGLTopology() const { return m_OpenGLPrim; }
	private:
		PrimitiveTopology m_Prim;
		Viewport m_Viewport;
		BlendFunction m_BlendFunc;

		GLenum m_OpenGLPrim;
	};

}

#endif