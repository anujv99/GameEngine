#pragma once

#ifdef ENGINE_OPENGL

#include "graphics/framebuffer.h"

#include "opengltexture.h"

namespace prev {

	class OpenGLFramebuffer : public Framebuffer {
	public:
		OpenGLFramebuffer(Vec2i size, TextureFormat format, FramebufferFlags flags);
		~OpenGLFramebuffer();
	public:
		virtual void Bind() override;
		virtual void UnBind() override;
		virtual void Resolve() override;
		virtual void Clear() override;
		virtual void Clear(Vec4 color) override;
		virtual Vec2i GetSize() const override;
		virtual StrongHandle<Texture2D> GetTexture() override;
		virtual StrongHandle<Texture2D> GetDepthTexture() override;
	public:
		StrongHandle<Texture2D> m_Tex;
		StrongHandle<Texture2D> m_DepthTex;
		GLuint m_ID;
		Vec2i m_Size;
		// For MSAA
		bool m_IsMultisampled;
		GLuint m_MSAAFbo;
		GLuint m_MSAATexID;
	};

}

#endif //ENGINE_OPENGL