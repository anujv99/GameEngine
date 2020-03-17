#include "font.h"

#include <cstring>
#include <freetype-gl.h>

namespace prev {

	static constexpr const pvuint TEXTURE_SIZE_X = 128;
	static constexpr const pvuint TEXTURE_SIZE_Y = 128;

	pvfloat PixelsToPoints(pvfloat pixels) {
		return static_cast<pvuint>(pixels * 0.75f);
	}

	pvfloat PointsToPixels(pvfloat pixels) {
		return static_cast<pvuint>(pixels * 1.333333333f);
	}

	Font::Font(pvstring file, pvfloat points) : m_Points(points) {
		ASSERTM(FileUtils::FileExists(file), "Font file not found");

		auto atlas = ftgl::texture_atlas_new(TEXTURE_SIZE_X, TEXTURE_SIZE_Y, 1);
		auto font = ftgl::texture_font_new_from_file(atlas, static_cast<pvfloat>(points), file.c_str());

		pvfloat pixels = PointsToPixels(points);

		for (pvuchar i = 0; i < 128u; i++) {
			char ch = static_cast<char>(i);
			auto glyph = ftgl::texture_font_get_glyph(font, &ch);
			ASSERT(glyph);
			Character character;
			character.CharCode				= i;
			character.Offset				= Vec2(glyph->offset_x, glyph->offset_y) / pixels;
			character.Size					= Vec2(glyph->width, glyph->height) / pixels;
			character.TexCoordTopLeft		= Vec2(glyph->s0, glyph->t0);
			character.TexCoordBottomRight	= Vec2(glyph->s1, glyph->t1 - 0.000001f);
			character.XAdvance				= glyph->advance_x / pixels;

			m_Characters.push_back(character);
		}

		TextureParams params;
		params.Filtering = TextureFiltering::NEAREST;
		params.InternalFormat = TextureFormat::RGBA;

		pvubyte * arr = new pvubyte[TEXTURE_SIZE_X * TEXTURE_SIZE_Y * 4];
		for (pvsizet i = 0, j = 0; i < TEXTURE_SIZE_X * TEXTURE_SIZE_Y * 4; i += 4, j++) {
			arr[i + 0] = atlas->data[j];
			arr[i + 1] = atlas->data[j];
			arr[i + 2] = atlas->data[j];
			arr[i + 3] = atlas->data[j];
		}

		pvubyte * buff = new pvubyte[TEXTURE_SIZE_X * 4u];

		if (GraphicsContext::Ref().GetAPI() == GraphicsAPI::API_OPENGL) {
			for (pvsizet j = 0; j < TEXTURE_SIZE_Y / 2; j++) {
				pvubyte * begin = arr + TEXTURE_SIZE_X * j * 4u;
				pvubyte * end = arr + (TEXTURE_SIZE_Y - j - 1) * TEXTURE_SIZE_X * 4u;

				std::memcpy(buff, begin, TEXTURE_SIZE_X * 4u);
				std::memcpy(begin, end, TEXTURE_SIZE_X * 4u);
				std::memcpy(end, buff, TEXTURE_SIZE_X * 4u);
			}
		}

		delete[] buff;

		m_Texture = Texture2D::Create(Vec2i(TEXTURE_SIZE_X, TEXTURE_SIZE_Y), params);
		m_Texture->SetData(arr, TEXTURE_SIZE_X * TEXTURE_SIZE_Y * 4);

		delete[] arr;

		ftgl::texture_font_delete(font);
		ftgl::texture_atlas_delete(atlas);
	}

	Font::~Font() {}

	pvfloat Font::GetWidth(pvstring text) const {
		pvfloat width = 0.0f;
		for (int i = 0; i < text.size(); i++) {
			if (i == text.size() - 1) {
				width += m_Characters[static_cast<pvsizet>(text[i])].Size.x;
			} else {
				width += m_Characters[static_cast<pvsizet>(text[i])].XAdvance;
			}
		}
		return width;
	}

	pvfloat Font::GetHeight(pvstring text) const {
		pvfloat min = 0.0f;
		pvfloat max = 0.0f;
		for (int i = 0; i < text.size(); i++) {
			const Character & c = m_Characters[static_cast<pvsizet>(text[i])];
			pvfloat height = c.Size.y;
			pvfloat offset = c.Offset.y - height;
			if (offset < min)
				min = offset;
			if (height > max)
				max = height;
		}
		return Abs(min) + Abs(max);
	}

	pvint Font::GetWidthPixels(pvstring text) const {
		pvint pixels = PointsToPixels(m_Points);

		pvint width = 0;
		for (int i = 0; i < text.size(); i++) {
			if (i == text.size() - 1) {
				width += m_Characters[static_cast<pvsizet>(text[i])].Size.x * pixels;
			} else {
				width += m_Characters[static_cast<pvsizet>(text[i])].XAdvance * pixels;
			}
		}
		return width;

	}

	pvint Font::GetHeightPixels(pvstring text) const {
		pvint pixels = PointsToPixels(m_Points);

		pvint min = 0.0f;
		pvint max = 0.0f;
		for (int i = 0; i < text.size(); i++) {
			const Character & c = m_Characters[static_cast<pvsizet>(text[i])];
			pvint height = c.Size.y * pixels;
			pvint offset = c.Offset.y * pixels - height;
			if (offset < min)
				min = offset;
			if (height > max)
				max = height;
		}
		return Abs(min) + Abs(max);
	}

}