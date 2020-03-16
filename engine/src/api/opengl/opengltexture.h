#pragma once

#include <glad/glad.h>

#include "graphics/texture.h"

namespace prev {

	GLenum GetTextureFormat(TextureFormat format);
	GLenum GetTextureInternalFormat(TextureFormat format);
	GLenum GetTextureFilter(TextureFiltering filter);
	GLenum GetTextureAddressMode(TextureWrapping wrapping);
	pvuint GetNumBytes(TextureFormat format);

	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(Vec2i size, TextureParams params);
		OpenGLTexture2D(GLuint id);
		~OpenGLTexture2D();
	public:
		virtual void Bind(pvuint slot) const override;
		virtual void UnBind() const override;
		virtual void SetData(const pvubyte * pixels, pvsizet size) override;

		GLuint GetID() const { return m_ID; }
	private:
		GLuint m_ID;
		GLuint m_Width, m_Height;
		GLuint m_BytesPerPixel;
		GLenum m_Format, m_InternalFormat;
	};

}