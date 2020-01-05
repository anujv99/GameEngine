#include "opengltexture.h"

namespace prev {

	GLenum GetTextureInternalFormat(TextureFormat format) {
		switch (format) {
		case prev::TextureFormat::R:	return GL_R8;
		case prev::TextureFormat::RG:	return GL_RG8;
		case prev::TextureFormat::RGB:	return GL_RGB8;
		case prev::TextureFormat::RGBA:	return GL_RGBA8;
		default:
		{
			ASSERTM(false, "[OpenGL] Invalid Texture format")
		}
		}
	}

	GLenum GetTextureFormat(TextureFormat format) {
		switch (format) {
		case prev::TextureFormat::R:	return GL_LUMINANCE;
		case prev::TextureFormat::RG:	return GL_RG;
		case prev::TextureFormat::RGB:	return GL_RGB;
		case prev::TextureFormat::RGBA:	return GL_RGBA;
		default:
		{
			ASSERTM(false, "[OpenGL] Invalid Texture format")
		}
		}
	}

	GLenum GetTextureFilter(TextureFiltering filter) {
		switch (filter) {
		case prev::TextureFiltering::NEAREST: return GL_NEAREST;
		case prev::TextureFiltering::LINEAR:  return GL_LINEAR;
		default:
		{
			ASSERTM(false, "[OpenGL] Invalid Texture Filter");
		}
		}
	}

	GLenum GetTextureAddressMode(TextureWrapping wrapping) {
		switch (wrapping) {
		case prev::TextureWrapping::REPEAT:	return GL_REPEAT;
		case prev::TextureWrapping::CLAMP:	return GL_CLAMP;
		default:
		{
			ASSERTM(false, "[OpenGL] Invalid Texture Wrapping");
		}
		}
	}

	pvuint GetNumBytes(TextureFormat format) {
		switch (format) {
		case prev::TextureFormat::R:	return 1u;
		case prev::TextureFormat::RG:	return 2u;
		case prev::TextureFormat::RGB:	return 3u;
		case prev::TextureFormat::RGBA:	return 4u;
		default:
		{
			ASSERTM(false, "[OpenGL] Invalid Texture format")
		}
		}
	}

	DEFINE_OBJECT_OPENGL(Texture2D, Vec2i size, TextureParams params) {
		return dynamic_cast<Texture2D *>(new OpenGLTexture2D(size, params));
	}

	OpenGLTexture2D::OpenGLTexture2D(Vec2i size, TextureParams params) : m_ID(0u), m_Width(size.x), m_Height(size.y),
		m_InternalFormat(GL_INVALID_ENUM), m_Format(GL_INVALID_ENUM), m_BytesPerPixel(0u) {
		ASSERT(size.x > 0 && size.y > 0);
		glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);

		glBindTexture(GL_TEXTURE_2D, m_ID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetTextureAddressMode(params.Wrapping));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetTextureAddressMode(params.Wrapping));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetTextureFilter(params.Filtering));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetTextureFilter(params.Filtering));

		m_InternalFormat = GetTextureInternalFormat(params.InternalFormat);
		m_Format = GetTextureFormat(params.InternalFormat);
		m_BytesPerPixel = GetNumBytes(params.InternalFormat);

		GLsizei xSize = static_cast<GLsizei>(size.x);
		GLsizei ySize = static_cast<GLsizei>(size.y);

		glTextureStorage2D(m_ID, 1, m_InternalFormat, xSize, ySize);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	OpenGLTexture2D::~OpenGLTexture2D() {
		glDeleteTextures(1, &m_ID);
	}

	void OpenGLTexture2D::Bind(pvuint slot) const {
		glBindTextureUnit(static_cast<GLuint>(slot), m_ID);
	}

	void OpenGLTexture2D::UnBind() const {}

	void OpenGLTexture2D::SetData(const pvubyte * pixels, pvsizet size) {
		ASSERT(pixels);
		ASSERT(size > 0u);
		if (size != static_cast<pvsizet>(m_BytesPerPixel) * static_cast<pvsizet>(m_Width) * static_cast<pvsizet>(m_Height)) {
			LOG_WARN("[OpenGL] Texture2D::SetData expects size to match the size of complete texture."
				"The size given is not equal to the size of the texture and may cause undefined behaviour");
		}
		glBindTexture(GL_TEXTURE_2D, m_ID);
		glTexSubImage2D(GL_TEXTURE_2D, 0u, 0u, 0u, m_Width, m_Height, m_Format, GL_UNSIGNED_BYTE, pixels);
	}

}