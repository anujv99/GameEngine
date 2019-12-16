#pragma once

#include "graphics/renderstate.h"

#include "directxhelper.h"

namespace prev {

	D3D_PRIMITIVE_TOPOLOGY GetDirectXType(PrimitiveTopology topology);

	class DirectXRenderState : public RenderState {
	public:
		DirectXRenderState();
		~DirectXRenderState() {}
	public:
		virtual void SetTopology(PrimitiveTopology topology) override;
		virtual void SetViewport(Viewport viewport) override;

		virtual PrimitiveTopology GetTopology() const override;
		virtual Viewport GetViewport() const override;
	private:
		PrimitiveTopology m_Prim;
		Viewport m_Viewport;
	};

}