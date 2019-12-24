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
			ASSERTM(false, "Invalid Primitive Topology");
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
	}

	PrimitiveTopology DirectXRenderState::GetTopology() const {
		return m_Prim;
	}

	Viewport DirectXRenderState::GetViewport() const {
		return m_Viewport;
	}

}

#endif