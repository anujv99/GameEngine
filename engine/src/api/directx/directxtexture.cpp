#include "directxtexture.h"

namespace prev {

	DXGI_FORMAT GetTextureFormat(TextureFormat format) {
		switch (format) {
		case prev::TextureFormat::R:	return DXGI_FORMAT_R8_UNORM;
		case prev::TextureFormat::RG:	return DXGI_FORMAT_R8G8_UNORM;
		case prev::TextureFormat::RGB:	return DXGI_FORMAT_R8G8B8A8_UNORM;
		case prev::TextureFormat::RGBA:	return DXGI_FORMAT_R8G8B8A8_UNORM;
		default:
		{
			ASSERTM(false, "[DirectX] Invalid Texture format");
		}
		}
	}

	D3D11_FILTER GetTextureFilter(TextureFiltering filter) {
		switch (filter) {
		case prev::TextureFiltering::LINEAR:	return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		case prev::TextureFiltering::NEAREST:	return D3D11_FILTER_MIN_MAG_MIP_POINT;
		default:
		{
			ASSERTM(false, "[DirectX] Invalid Texture Filter");
		}
		}
	}

	D3D11_TEXTURE_ADDRESS_MODE GetTextureAddressMode(TextureWrapping wrapping) {
		switch (wrapping) {
		case prev::TextureWrapping::REPEAT:	return D3D11_TEXTURE_ADDRESS_WRAP;
		case prev::TextureWrapping::CLAMP:	return D3D11_TEXTURE_ADDRESS_CLAMP;
		default:
		{
			ASSERTM(false, "[DirectX] Invalid Texture Wrapping");
		}
		}
	}

	pvuint GetNumComponents(TextureFormat format) {
		switch (format) {
		case prev::TextureFormat::R:	return 1u;
		case prev::TextureFormat::RG:	return 2u;
		case prev::TextureFormat::RGB:	return 3u;
		case prev::TextureFormat::RGBA:	return 4u;
		default:
		{
			ASSERTM(false, "[DirectX] Invalid Texture format");
		}
		}
	}

	DEFINE_OBJECT_DIRECTX(Texture2D, Vec2i size, TextureParams params) {
		return dynamic_cast<Texture2D *>(new DirectXTexture2D(size, params));
	}

	DirectXTexture2D::DirectXTexture2D(Vec2i size, TextureParams params) :  m_Width(size.x), m_Height(size.y), m_BytesPerPixel(0u) {
		ASSERT(size.x > 0 && size.y > 0);
		m_BytesPerPixel = GetNumComponents(params.InternalFormat);
		CreateTexture(size, params);
		CreateSampleState(params);
	}

	DirectXTexture2D::DirectXTexture2D(D3D11_TEXTURE2D_DESC desc) : m_Sampler(nullptr), m_Width(desc.Width), m_Height(desc.Height) {
		desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;


		Microsoft::WRL::ComPtr<ID3D11Texture2D> d3dtexure;
		HRESULT hr = GetDevice()->CreateTexture2D(&desc, nullptr, d3dtexure.GetAddressOf());
		ASSERTM(hr == S_OK, "[DirectX] Failed to create texture2d");

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc		= {};
		srvDesc.Format								= desc.Format;
		srvDesc.ViewDimension						= D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip			= 0;
		srvDesc.Texture2D.MipLevels					= 1;

		hr = GetDevice()->CreateShaderResourceView(d3dtexure.Get(), &srvDesc, m_View.GetAddressOf());
		ASSERTM(hr == S_OK, "[DirectX] Failed to create texture2d shader resource view");

		TextureParams params;
		params.Filtering		= TextureFiltering::NEAREST;
		params.Wrapping			= TextureWrapping::CLAMP;
		params.InternalFormat	= TextureFormat::RGBA;

		CreateSampleState(params);
	}

	DirectXTexture2D::DirectXTexture2D(D3D11_TEXTURE2D_DESC desc, D3D11_SHADER_RESOURCE_VIEW_DESC vDesc) :
		m_Sampler(nullptr), m_Width(desc.Width), m_Height(desc.Height) {
		desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> d3dtexure;
		HRESULT hr = GetDevice()->CreateTexture2D(&desc, nullptr, d3dtexure.GetAddressOf());
		ASSERTM(hr == S_OK, "[DirectX] Failed to create texture2d");

		hr = GetDevice()->CreateShaderResourceView(d3dtexure.Get(), &vDesc, m_View.GetAddressOf());
		ASSERTM(hr == S_OK, "[DirectX] Failed to create texture2d shader resource view");

		TextureParams params;
		params.Filtering		= TextureFiltering::NEAREST;
		params.Wrapping			= TextureWrapping::CLAMP;
		params.InternalFormat	= TextureFormat::RGBA;

		CreateSampleState(params);
	}

	DirectXTexture2D::~DirectXTexture2D() {}

	void DirectXTexture2D::Bind(pvuint slot) const {
		GetDeviceContext()->PSSetSamplers(static_cast<UINT>(slot), 1, m_Sampler.GetAddressOf());
		GetDeviceContext()->PSSetShaderResources(static_cast<UINT>(slot), 1, m_View.GetAddressOf());
	}

	void DirectXTexture2D::UnBind() const {}

	void DirectXTexture2D::SetData(const pvubyte * pixels, pvsizet size) {
		PV_PROFILE_FUNCTION();
		ASSERT(pixels);
		ASSERT(size > 0u);
		if (size != static_cast<pvsizet>(m_BytesPerPixel) * static_cast<pvsizet>(m_Width) * static_cast<pvsizet>(m_Height)) {
			LOG_WARN("[DirectX] Texture2D::SetData expects size to match the size of complete texture."
				"The size given is not equal to the size of the texture and may cause undefined behaviour");
		}
		D3D11_MAPPED_SUBRESOURCE msr = {};
		ComPtr<ID3D11Resource> textureResource;
		m_View->GetResource(textureResource.GetAddressOf());

		HRESULT hr = GetDeviceContext()->Map(textureResource.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr);
		ASSERTM(hr == S_OK, "[DirectX] Failed to map texture");

		pvubyte * mappedData = reinterpret_cast<pvubyte *>(msr.pData);
		
		if (m_BytesPerPixel == 3) {
			for (pvsizet i = 0; i < m_Height; i++) {
				pvubyte * ptr = mappedData;
				pvsizet index = 0;
				for (pvsizet j = 0; j < m_Width; j++) {
					ptr[index++] = *pixels++;
					ptr[index++] = *pixels++;
					ptr[index++] = *pixels++;
					ptr[index++] = 255;
				}
				mappedData += msr.RowPitch;
			}
		} else {
			for (pvsizet i = 0; i < m_Height; i++) {
				memcpy(mappedData, pixels, static_cast<pvsizet>(m_Width) * static_cast<pvsizet>(m_BytesPerPixel));
				mappedData += msr.RowPitch;
				pixels += static_cast<pvsizet>(m_Width) * static_cast<pvsizet>(m_BytesPerPixel);
			}
		}

		GetDeviceContext()->Unmap(textureResource.Get(), 0u);
	}

	prev::ComPtr<ID3D11Texture2D> DirectXTexture2D::GetTexture2D() {
		ComPtr<ID3D11Resource> res;
		m_View->GetResource(res.GetAddressOf());

		ComPtr<ID3D11Texture2D> tex;
		HRESULT hr = res->QueryInterface<ID3D11Texture2D>(tex.GetAddressOf());
		ASSERTM(hr == S_OK, "[DirectX] Failed to get texture resource");

		return tex;
	}

	void DirectXTexture2D::CreateTexture(Vec2i size, TextureParams params) {
		D3D11_TEXTURE2D_DESC textureDesc	= {};
		textureDesc.Width					= static_cast<UINT>(size.x);
		textureDesc.Height					= static_cast<UINT>(size.y);
		textureDesc.MipLevels				= 1;
		textureDesc.ArraySize				= 1;
		textureDesc.Format					= GetTextureFormat(params.InternalFormat);
		textureDesc.SampleDesc.Count		= 1;
		textureDesc.SampleDesc.Quality		= 0;
		textureDesc.Usage					= D3D11_USAGE_DYNAMIC;
		textureDesc.BindFlags				= D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags			= D3D11_CPU_ACCESS_WRITE;
		textureDesc.MiscFlags				= 0;

		ComPtr<ID3D11Texture2D> texture;
		HRESULT hr = GetDevice()->CreateTexture2D(&textureDesc, nullptr, texture.GetAddressOf());
		ASSERTM(hr == S_OK, "[DirectX] Failed to create Texture2D");

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc		= {};
		srvDesc.Format								= textureDesc.Format;
		srvDesc.ViewDimension						= D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip			= 0;
		srvDesc.Texture2D.MipLevels					= 1;

		hr = GetDevice()->CreateShaderResourceView(texture.Get(), &srvDesc, m_View.GetAddressOf());
		ASSERTM(hr == S_OK, "[DirectX] Failed to create shader resource view");
	}

	void DirectXTexture2D::CreateSampleState(TextureParams params) {
		D3D11_SAMPLER_DESC sd	= {};
		sd.Filter				= GetTextureFilter(params.Filtering);
		sd.AddressU				= GetTextureAddressMode(params.Wrapping);
		sd.AddressV				= GetTextureAddressMode(params.Wrapping);
		sd.AddressW				= GetTextureAddressMode(params.Wrapping);

		HRESULT hr = GetDevice()->CreateSamplerState(&sd, m_Sampler.GetAddressOf());
		ASSERTM(hr == S_OK, "[DirectX] Failed to create shader resource view");
	}

}