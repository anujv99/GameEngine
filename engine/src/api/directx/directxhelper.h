#pragma once

#include <d3d11.h>
#include <dxgi.h>

#include "directxcontext.h"

namespace prev {

	inline static ComPtr<ID3D11Device> GetDevice() {
		DirectXContext * con = dynamic_cast<DirectXContext *>(GraphicsContext::Get());
		return con->GetDevice();
	}

	inline static ComPtr<ID3D11DeviceContext> GetDeviceContext() {
		DirectXContext * con = dynamic_cast<DirectXContext *>(GraphicsContext::Get());
		return con->GetDeviceContext();
	}

}