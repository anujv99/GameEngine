#pragma once

#include "../common/handledobject.h"
#include "../common/types.h"
#include "../math/math.h"

#include "graphicscontext.h"
#include "texture.h"

#define BIT(X) (1 << X)

namespace prev {

	enum FramebufferFlags {
		FRAMEBUFFER_NONE					= 0u,
		FRAMEBUFFER_ALLOCATE_DEPTH_BUFFER	= BIT(0),
		FRAMEBUFFER_MSAA_16x				= BIT(1),
		FRAMEBUFFER_MSAA_8x					= BIT(2),
		FRAMEBUFFER_MSAA_4x					= BIT(3),
		FRAMEBUFFER_MSAA_2x					= BIT(4),
	};

	class Framebuffer : public HandledObject<Framebuffer> {
		friend class StrongHandle<Framebuffer>;
	protected:
		Framebuffer() {}
		virtual ~Framebuffer() {}
	public:
		virtual void Bind() = 0;
		virtual void UnBind() = 0;
		virtual void Resolve() = 0;
		virtual void Clear() = 0;
		virtual void Clear(Vec4 color) = 0;
		virtual Vec2i GetSize() const = 0;
		virtual StrongHandle<Texture2D> GetTexture() = 0;
		virtual StrongHandle<Texture2D> GetDepthTexture() = 0;

		DECLARE_OBJECT(Framebuffer, Vec2i size, TextureFormat format, FramebufferFlags flags = FRAMEBUFFER_NONE);
	
	};

}