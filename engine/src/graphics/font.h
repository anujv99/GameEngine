#pragma once

#include <vector>

#include "../common/types.h"
#include "../common/handledobject.h"
#include "../math/math.h"
#include "../utils/fileutils.h"

#include "texture.h"

namespace prev {

	struct Label {
		enum class Alignment {
			CENTER,
			LEFT,
			RIGHT
		};

		pvstring Text;
		Vec2 Scale			= Vec2(1.0f);
		Vec2 Pos			= Vec2(0.0f);
		Vec4 Color			= Vec4(1.0f);
		Alignment Align		= Alignment::CENTER;
	};

	class Font : public HandledObject<Font> {
	public:
		struct Character {
			pvuchar CharCode;
			Vec2 TexCoordTopLeft, TexCoordBottomRight;
			Vec2 Size;
			Vec2 Offset;
			pvfloat XAdvance;
		};
	public:
		Font(pvstring file, pvfloat points);
		~Font();
	public:
		pvfloat GetWidth(pvstring text) const;
		pvfloat GetHeight(pvstring text) const;
		inline Vec2 GetSize(pvstring text) const { return Vec2(GetWidth(text), GetHeight(text)); }

		pvint GetWidthPixels(pvstring text) const;
		pvint GetHeightPixels(pvstring text) const;
		inline Vec2i GetSizePixels(pvstring text) const { return Vec2i(GetWidthPixels(text), GetHeightPixels(text)); }

		inline StrongHandle<Texture2D> GetTexture() { return m_Texture; }
		inline const Character & GetCharacter(pvuchar charCode) const { return m_Characters[static_cast<pvsizet>(charCode)]; }
	private:
		StrongHandle<Texture2D> m_Texture;
		std::vector<Character> m_Characters;
		pvfloat m_Points;
	};

}