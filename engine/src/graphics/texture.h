#pragma once

#include "helper.h"
#include "graphicscontext.h"

#include "../common/handledobject.h"
#include "../math/math.h"

namespace prev {

	enum class TextureFiltering {
		NEAREST,
		LINEAR,
	};

	enum class TextureWrapping {
		REPEAT,
		CLAMP
	};

	enum class TextureFormat {
		R,
		RG,
		RGB,
		RGBA
	};

	struct TextureParams {
		TextureFiltering Filtering			= TextureFiltering::LINEAR;
		TextureWrapping Wrapping			= TextureWrapping::CLAMP;
		TextureFormat InternalFormat		= TextureFormat::RGBA;
	};

	class Texture2D : public HandledObject<Texture2D> {
		friend class StrongHandle<Texture2D>;
	protected:
		Texture2D() {}
		virtual ~Texture2D() {}
	public:
		virtual void Bind(pvuint slot) const = 0;
		virtual void UnBind() const = 0;
		virtual void SetData(const pvubyte * pixels, pvsizet size) = 0;

		DECLARE_OBJECT(Texture2D, Vec2i size, TextureParams params);
	};

}