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

	class RenderState : public Singleton<RenderState> {
		friend class Singleton<RenderState>;
	protected:
		RenderState() {}
		virtual ~RenderState() {}
	public:
		virtual void SetTopology(PrimitiveTopology topology) = 0;
		virtual void SetViewport(Viewport viewport) = 0;

		virtual PrimitiveTopology GetTopology() const = 0;
		virtual Viewport GetViewport() const = 0;

		DECLARE_OBJECT(RenderState);
	};

	DEFINE_SINGLE_OBJECT(RenderState);

}