#include "directxrenderstate.h"

#ifdef ENGINE_DIRECTX

namespace prev {

	D3D_PRIMITIVE_TOPOLOGY GetDirectXType(PrimitiveTopology topology) {
		switch (topology) {
		case prev::PrimitiveTopology::TOPOLOGY_POINT:					return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
		case prev::PrimitiveTopology::TOPOLOGY_LINE:					return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
		case prev::PrimitiveTopology::TOPOLOGY_LINE_ADJ:				return D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ;
		case prev::PrimitiveTopology::TOPOLOGY_LINE_STRIP:				return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
		case prev::PrimitiveTopology::TOPOLOGY_LINE_STRIP_ADJ:			return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ;
		case prev::PrimitiveTopology::TOPOLOGY_TRIANGLE:				return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		case prev::PrimitiveTopology::TOPOLOGY_TRIANGLE_ADJ:			return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;
		case prev::PrimitiveTopology::TOPOLOGY_TRIANGLE_STRIP:			return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
		case prev::PrimitiveTopology::TOPOLOGY_TRIANGLE_STRIP_ADJ:		return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ;
		default:
		{
			ASSERTM(false, "[DirectX] Invalid Primitive Topology");
		}
		}
	}

	D3D11_BLEND GetDirectXType(BlendColorOption option) {
		switch (option) {
		case prev::BlendColorOption::BLEND_ZERO:			return D3D11_BLEND_ZERO;
		case prev::BlendColorOption::BLEND_ONE:				return D3D11_BLEND_ONE;
		case prev::BlendColorOption::BLEND_SRC_COLOR:		return D3D11_BLEND_SRC_COLOR;
		case prev::BlendColorOption::BLEND_INV_SRC_COLOR:	return D3D11_BLEND_INV_SRC_COLOR;
		case prev::BlendColorOption::BLEND_DEST_COLOR:		return D3D11_BLEND_DEST_COLOR;
		case prev::BlendColorOption::BLEND_INV_DEST_COLOR:	return D3D11_BLEND_INV_DEST_COLOR;
		case prev::BlendColorOption::BLEND_SRC_ALPHA:		return D3D11_BLEND_SRC_ALPHA;
		case prev::BlendColorOption::BLEND_INV_SRC_ALPHA:	return D3D11_BLEND_INV_SRC_ALPHA;
		case prev::BlendColorOption::BLEND_DEST_ALPHA:		return D3D11_BLEND_DEST_ALPHA;
		case prev::BlendColorOption::BLEND_INV_DEST_ALPHA:	return D3D11_BLEND_INV_DEST_ALPHA;
		default:
		{
			ASSERTM(false, "[DirectX] Invalid BlendColorOption");
		}
		}
	}

	D3D11_BLEND GetDirectXType(BlendAlphaOption option) {
		switch (option) {
		case prev::BlendAlphaOption::BLEND_ZERO:			return D3D11_BLEND_ZERO;
		case prev::BlendAlphaOption::BLEND_ONE:				return D3D11_BLEND_ONE;
		case prev::BlendAlphaOption::BLEND_SRC_ALPHA:		return D3D11_BLEND_SRC_ALPHA;
		case prev::BlendAlphaOption::BLEND_INV_SRC_ALPHA:	return D3D11_BLEND_INV_SRC_ALPHA;
		case prev::BlendAlphaOption::BLEND_DEST_ALPHA:		return D3D11_BLEND_DEST_ALPHA;
		case prev::BlendAlphaOption::BLEND_INV_DEST_ALPHA:	return D3D11_BLEND_INV_DEST_ALPHA;
		default:
		{
			ASSERTM(false, "[DirectX] Invalid BlendAlphaOption");
		}
		}
	}

	D3D11_BLEND_OP GetDirectXType(BlendOperation operation) {
		switch (operation) {
		case prev::BlendOperation::BLEND_OP_ADD:			return D3D11_BLEND_OP_ADD;
		case prev::BlendOperation::BLEND_OP_SUBTRACT:		return D3D11_BLEND_OP_SUBTRACT;
		case prev::BlendOperation::BLEND_OP_REV_SUBTRACT:	return D3D11_BLEND_OP_REV_SUBTRACT;
		case prev::BlendOperation::BLEND_OP_MIN:			return D3D11_BLEND_OP_MIN;
		case prev::BlendOperation::BLEND_OP_MAX:			return D3D11_BLEND_OP_MAX;
		default:
		{
			ASSERTM(false, "[DirectX] Invalid BlendOperation");
		}
		}
	}

	DEFINE_SINGLE_OBJECT_DIRECTX(RenderState) {
		return dynamic_cast<RenderState *>(new DirectXRenderState());
	}

	DirectXRenderState::DirectXRenderState() : m_Prim(), m_Viewport() {}

	void DirectXRenderState::SetTopology(PrimitiveTopology topology) {
		PV_PROFILE_FUNCTION();
		if (m_Prim == topology) return;
		GetDeviceContext()->IASetPrimitiveTopology(GetDirectXType(topology));
		m_Prim = topology;
	}

	void DirectXRenderState::SetViewport(Viewport viewport) {
		PV_PROFILE_FUNCTION();
		D3D11_VIEWPORT vp;
		vp.TopLeftX		= viewport.TopLeftX;
		vp.TopLeftY		= viewport.TopLeftY;
		vp.Width		= viewport.Width;
		vp.Height		= viewport.Height;
		vp.MinDepth		= viewport.MinDepth;
		vp.MaxDepth		= viewport.MaxDepth;

		GetDeviceContext()->RSSetViewports(1, &vp);
		m_Viewport = viewport;

		dynamic_cast<DirectXContext *>(GraphicsContext::Get())->ChangeResolution(
			static_cast<UINT>(viewport.Width),
			static_cast<UINT>(viewport.Height)
		);
	}

	void DirectXRenderState::SetBlendFunction(BlendFunction func) {
		D3D11_BLEND_DESC bd;
		bd.AlphaToCoverageEnable = FALSE;
		bd.IndependentBlendEnable = FALSE;
		bd.RenderTarget[0].BlendEnable = func.EnableBlending ? TRUE : FALSE;

		if (func.EnableBlending) {
			bd.RenderTarget[0].SrcBlend						= GetDirectXType(func.SrcColor);
			bd.RenderTarget[0].DestBlend					= GetDirectXType(func.DestColor);
			bd.RenderTarget[0].BlendOp						= GetDirectXType(func.ColorOperation);
			bd.RenderTarget[0].SrcBlendAlpha				= GetDirectXType(func.SrcAlpha);
			bd.RenderTarget[0].DestBlendAlpha				= GetDirectXType(func.DestAlpha);
			bd.RenderTarget[0].BlendOpAlpha					= GetDirectXType(func.AlphaOperation);
			bd.RenderTarget[0].RenderTargetWriteMask		= D3D11_COLOR_WRITE_ENABLE_ALL;
		}

		ComPtr<ID3D11BlendState> blendState;

		HRESULT hr = GetDevice()->CreateBlendState(&bd, blendState.GetAddressOf());
		ASSERTM(hr == S_OK, "[DirectX] Failed to create blend state");

		GetDeviceContext()->OMSetBlendState(blendState.Get(), nullptr, 0xffffffff);

		m_Func = func;
	}

	PrimitiveTopology DirectXRenderState::GetTopology() const {
		return m_Prim;
	}

	Viewport DirectXRenderState::GetViewport() const {
		return m_Viewport;
	}

	BlendFunction DirectXRenderState::GetBlendFunction() const {
		return m_Func;
	}

}

#endif