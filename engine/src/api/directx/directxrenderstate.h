#pragma once

#ifdef ENGINE_DIRECTX

#include "graphics/renderstate.h"

#include "directxhelper.h"

namespace prev {

	D3D_PRIMITIVE_TOPOLOGY GetDirectXType(PrimitiveTopology topology);
	D3D11_BLEND GetDirectXType(BlendColorOption option);
	D3D11_BLEND GetDirectXType(BlendAlphaOption option);
	D3D11_BLEND_OP GetDirectXType(BlendOperation operation);


	class DirectXRenderState : public RenderState {
	public:
		DirectXRenderState();
		~DirectXRenderState() {}
	public:
		virtual void SetTopology(PrimitiveTopology topology) override;
		virtual void SetViewport(Viewport viewport) override;
		virtual void SetBlendFunction(BlendFunction func) override;

		virtual PrimitiveTopology GetTopology() const override;
		virtual Viewport GetViewport() const override;
		virtual BlendFunction GetBlendFunction() const override;
	private:
		PrimitiveTopology m_Prim;
		Viewport m_Viewport;
		BlendFunction m_Func;
	};

}

#endif