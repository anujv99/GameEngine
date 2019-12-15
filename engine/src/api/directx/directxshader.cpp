#include "directxshader.h"

#ifdef ENGINE_DIRECTX

#define VERTEX_SHADER_MODEL "vs_5_0"
#define PIXEL_SHADER_MODEL "ps_5_0"

namespace prev {

	// ------------- VERTEX SHADER -------------

	DEFINE_OBJECT_DIRECTX(VertexShader, const pvstring & source) {
		return dynamic_cast<VertexShader *>(new DirectXVertexShader(source));
	}

	DirectXVertexShader::DirectXVertexShader(const pvstring & source) {
		UINT flags = 0;
		#ifdef ENGINE_DEBUG
		flags |= D3DCOMPILE_DEBUG;
		#endif
		Microsoft::WRL::ComPtr<ID3DBlob> errorMessages;

		HRESULT hr = D3DCompile(
			source.c_str(),
			source.length(),
			nullptr,
			nullptr,
			nullptr,
			"main",
			VERTEX_SHADER_MODEL,
			flags,
			0u,
			m_ShaderBytecode.GetAddressOf(),
			errorMessages.GetAddressOf()
		);

		if (FAILED(hr)) {
			std::string errorMessage = (const char *)errorMessages->GetBufferPointer();
			LOG_ERROR("Failed to compile vertex shader:");
			LOG_ERROR("%s", errorMessage.c_str());
			ASSERTM(false, "Fatal Error");
		}

		hr = GetDevice()->CreateVertexShader(
			m_ShaderBytecode->GetBufferPointer(),
			m_ShaderBytecode->GetBufferSize(),
			nullptr, m_VertexShader.GetAddressOf()
		);

		if (FAILED(hr)) {
			LOG_ERROR("Unable to create vertex shader");
			ASSERTM(false, "Fatal Error");
		}
	}

	DirectXVertexShader::~DirectXVertexShader() {}

	void DirectXVertexShader::Bind() {
		GetDeviceContext()->VSSetShader(m_VertexShader.Get(), nullptr, 0);
	}

	// ------------- FRAGMENT SHADER -------------

	DEFINE_OBJECT_DIRECTX(FragmentShader, const pvstring & source) {
		return dynamic_cast<FragmentShader *>(new DirectXPixelShader(source));
	}

	DirectXPixelShader::DirectXPixelShader(const pvstring & source) {
		UINT flags = 0;
		#ifdef ENGINE_DEBUG
		flags |= D3DCOMPILE_DEBUG;
		#endif
		Microsoft::WRL::ComPtr<ID3DBlob> errorMessages;

		HRESULT hr = D3DCompile(
			source.c_str(),
			source.length(),
			nullptr,
			nullptr,
			nullptr,
			"main",
			PIXEL_SHADER_MODEL,
			flags,
			0u,
			m_ShaderBytecode.GetAddressOf(),
			errorMessages.GetAddressOf()
		);

		if (FAILED(hr)) {
			std::string errorMessage = (const char *)errorMessages->GetBufferPointer();
			LOG_ERROR("Failed to compile pixel shader:");
			LOG_ERROR("%s", errorMessage.c_str());
			ASSERTM(false, "Fatal Error");
		}

		hr = GetDevice()->CreatePixelShader(
			m_ShaderBytecode->GetBufferPointer(),
			m_ShaderBytecode->GetBufferSize(),
			nullptr, m_PixelShader.GetAddressOf()
		);

		if (FAILED(hr)) {
			LOG_ERROR("Unable to create pixel shader");
			ASSERTM(false, "Fatal Error");
		}
	}

	DirectXPixelShader::~DirectXPixelShader() {}

	void DirectXPixelShader::Bind() {
		GetDeviceContext()->PSSetShader(m_PixelShader.Get(), nullptr, 0);
	}

	// ------------- SHADER PROGRAM -------------

	DEFINE_OBJECT_DIRECTX(ShaderProgram, StrongHandle<VertexShader> vShader, StrongHandle<FragmentShader> fShader) {
		return dynamic_cast<ShaderProgram *>(new DirectXShaderProgram(vShader, fShader));
	}

	DirectXShaderProgram::DirectXShaderProgram(StrongHandle<VertexShader> vShader, StrongHandle<FragmentShader> fShader) : 
		m_VS(vShader), m_FS(fShader) {}

	DirectXShaderProgram::~DirectXShaderProgram() {}

	void prev::DirectXShaderProgram::Bind() {
		DirectXVertexShader * vs = dynamic_cast<DirectXVertexShader *>(m_VS.Get());
		DirectXPixelShader * ps = dynamic_cast<DirectXPixelShader *>(m_FS.Get());
		vs->Bind();
		ps->Bind();
	}

}

#endif
