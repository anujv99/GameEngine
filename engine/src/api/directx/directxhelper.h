#pragma once

#ifdef ENGINE_DIRECTX

#include <d3d11.h>
#include <dxgi.h>

#include "directxcontext.h"

namespace prev {

	extern ComPtr<ID3D11Device> s_Device;
	extern ComPtr<ID3D11DeviceContext> s_DeviceContext;

	inline static ComPtr<ID3D11Device> GetDevice() {
		return s_Device;
	}

	inline static ComPtr<ID3D11DeviceContext> GetDeviceContext() {
		return s_DeviceContext;
	}

}

#endif