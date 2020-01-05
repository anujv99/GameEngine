#pragma once

#include "../common/singleton.h"

#include "graphicscontext.h"

namespace prev {

	enum class PrimitiveTopology {
		TOPOLOGY_POINT,
		TOPOLOGY_LINE,
		TOPOLOGY_LINE_ADJ,
		TOPOLOGY_LINE_STRIP,
		TOPOLOGY_LINE_STRIP_ADJ,
		TOPOLOGY_TRIANGLE,
		TOPOLOGY_TRIANGLE_ADJ,
		TOPOLOGY_TRIANGLE_STRIP,
		TOPOLOGY_TRIANGLE_STRIP_ADJ,
	};

	struct Viewport {
		pvfloat TopLeftX, TopLeftY;
		pvfloat Width, Height;
		pvfloat MinDepth, MaxDepth; //Used in DirectX. Don't know if it is used in OpenGL
	};

	enum class BlendColorOption {
		BLEND_ZERO,
		BLEND_ONE,
		BLEND_SRC_COLOR,
		BLEND_INV_SRC_COLOR,
		BLEND_DEST_COLOR,
		BLEND_INV_DEST_COLOR,
		BLEND_SRC_ALPHA,
		BLEND_INV_SRC_ALPHA,
		BLEND_DEST_ALPHA,
		BLEND_INV_DEST_ALPHA
	};

	enum class BlendAlphaOption {
		BLEND_ZERO,
		BLEND_ONE,
		BLEND_SRC_ALPHA,
		BLEND_INV_SRC_ALPHA,
		BLEND_DEST_ALPHA,
		BLEND_INV_DEST_ALPHA
	};

	enum class BlendOperation {
		BLEND_OP_ADD,
		BLEND_OP_SUBTRACT,
		BLEND_OP_REV_SUBTRACT,
		BLEND_OP_MIN,
		BLEND_OP_MAX
	};

	struct BlendFunction {
		pvbool EnableBlending = true;

		BlendColorOption SrcColor, DestColor;
		BlendAlphaOption SrcAlpha, DestAlpha;

		BlendOperation ColorOperation, AlphaOperation;
	};

	class RenderState : public Singleton<RenderState> {
		friend class Singleton<RenderState>;
	protected:
		RenderState() {}
		virtual ~RenderState() {}
	public:
		virtual void SetTopology(PrimitiveTopology topology) = 0;
		virtual void SetViewport(Viewport viewport) = 0;
		virtual void SetBlendFunction(BlendFunction func) = 0;

		virtual PrimitiveTopology GetTopology() const = 0;
		virtual Viewport GetViewport() const = 0;
		virtual BlendFunction GetBlendFunction() const = 0;

		DECLARE_OBJECT(RenderState);
	};

	DEFINE_SINGLE_OBJECT(RenderState);

}