#include "openglframebuffer.h"

#ifdef ENGINE_OPENGL

namespace prev {

	DEFINE_OBJECT_OPENGL(Framebuffer, Vec2i size, TextureFormat format, FramebufferFlags flags) {
		return dynamic_cast<OpenGLFramebuffer *>(new OpenGLFramebuffer(size, format, flags));
	}


	OpenGLFramebuffer::OpenGLFramebuffer(Vec2i size, TextureFormat format, FramebufferFlags flags) :
		m_Tex(nullptr), m_Size(size), m_ID(0u), m_IsMultisampled(false), m_MSAATexID(0u), m_DepthTex(nullptr) {
		ASSERTM(size.x > 0 && size.y > 0, "[OpenGL] Invalid size for framebuffer");

		glCreateFramebuffers(1, &m_ID);
		ASSERTM(m_ID > 0, "[OpenGL] Failed to create framebuffer");

		TextureParams params;
		params.Filtering = TextureFiltering::NEAREST;
		params.InternalFormat = format;
		params.Wrapping = TextureWrapping::CLAMP;

		m_Tex = Texture2D::Create(size, params);

		GLuint texID = dynamic_cast<OpenGLTexture2D *>(m_Tex.Get())->GetID();

		GLuint depthtexid = 0;

		if (flags & FRAMEBUFFER_ALLOCATE_DEPTH_BUFFER) {
			glCreateTextures(GL_TEXTURE_2D, 1, &depthtexid);
			ASSERTM(depthtexid != 0, "[OpenGL] Failed to create depth texture for framebuffer");

			glBindTexture(GL_TEXTURE_2D, depthtexid);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetTextureAddressMode(params.Wrapping));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetTextureAddressMode(params.Wrapping));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetTextureFilter(params.Filtering));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetTextureFilter(params.Filtering));

			glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, size.x, size.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			glBindTexture(GL_TEXTURE_2D, 0);

			m_DepthTex = new OpenGLTexture2D(depthtexid);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texID, 0);

		if (m_DepthTex != nullptr) {
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthtexid, 0);
		}

		ASSERTM(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "[OpenGL] Failed to create framebuffer");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		GLuint numSamples = 0u;

		if (flags & FRAMEBUFFER_MSAA_16x)
			numSamples = 16;
		else if (flags & FRAMEBUFFER_MSAA_8x)
			numSamples = 8;
		else if (flags & FRAMEBUFFER_MSAA_4x)
			numSamples = 4;
		else if (flags & FRAMEBUFFER_MSAA_2x)
			numSamples = 2;
		else return;

		m_IsMultisampled = true;

		glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &m_MSAATexID);
		ASSERTM(m_MSAATexID > 0u, "[OpenGL] Failed to create multisampled texture");

		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_MSAATexID);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, numSamples, GetTextureInternalFormat(format), size.x, size.y, false);

		glGenFramebuffers(1, &m_MSAAFbo);
		ASSERTM(m_ID > 0, "[OpenGL] Failed to create multisampled framebuffer");
		glBindFramebuffer(GL_FRAMEBUFFER, m_MSAAFbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_MSAATexID, 0);

		ASSERTM(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "[OpenGL] Failed to create multisampled framebuffer");

		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0u);
		glBindFramebuffer(GL_FRAMEBUFFER, 0u);
	}

	OpenGLFramebuffer::~OpenGLFramebuffer() {}

	void OpenGLFramebuffer::Bind() {
		if (m_IsMultisampled)
			glBindFramebuffer(GL_FRAMEBUFFER, m_MSAAFbo);
		else
			glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
	}

	void OpenGLFramebuffer::UnBind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resolve() {
		if (!m_IsMultisampled) {
			LOG_WARN("[OpenGL] Resolve is only required while using multisampled fbo");
			return;
		}
		// glBlitNamedFramebuffer not working on some system
		//glBlitNamedFramebuffer(m_MSAAFbo, m_ID, 0, 0, m_Size.x, m_Size.y, 0, 0, m_Size.x, m_Size.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_ID);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_MSAAFbo);
		glBlitFramebuffer(0, 0, m_Size.x, m_Size.y, 0, 0, m_Size.x, m_Size.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Clear() {
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void OpenGLFramebuffer::Clear(Vec4 color) {
		glClearColor(color.x, color.y, color.z, color.w);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	prevmath::Vec2i OpenGLFramebuffer::GetSize() const {
		return m_Size;
	}

	prev::StrongHandle<prev::Texture2D> OpenGLFramebuffer::GetTexture() {
		return m_Tex;
	}

	prev::StrongHandle<prev::Texture2D> OpenGLFramebuffer::GetDepthTexture() {
		return m_DepthTex;
	}

}

#endif //ENGINE_OPENGL