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
		PV_PROFILE_FUNCTION();
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

		LOG_INFO("[DirectX] Vertex shader successfully created");
	}

	DirectXVertexShader::~DirectXVertexShader() {}

	void DirectXVertexShader::Bind() {
		PV_PROFILE_FUNCTION();
		GetDeviceContext()->VSSetShader(m_VertexShader.Get(), nullptr, 0u);
	}

	// ------------- FRAGMENT SHADER -------------

	DEFINE_OBJECT_DIRECTX(FragmentShader, const pvstring & source) {
		return dynamic_cast<FragmentShader *>(new DirectXPixelShader(source));
	}

	DirectXPixelShader::DirectXPixelShader(const pvstring & source) {
		PV_PROFILE_FUNCTION();
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

		LOG_INFO("[DirectX] Fragment shader successfully created");
	}

	DirectXPixelShader::~DirectXPixelShader() {}

	void DirectXPixelShader::Bind() {
		PV_PROFILE_FUNCTION();
		GetDeviceContext()->PSSetShader(m_PixelShader.Get(), nullptr, 0u);
	}

	// ------------- SHADER PROGRAM -------------

	DEFINE_OBJECT_DIRECTX(ShaderProgram, StrongHandle<VertexShader> vShader, StrongHandle<FragmentShader> fShader) {
		return dynamic_cast<ShaderProgram *>(new DirectXShaderProgram(vShader, fShader));
	}

	DirectXShaderProgram::DirectXShaderProgram(StrongHandle<VertexShader> vShader, StrongHandle<FragmentShader> fShader) : 
		m_VS(dynamic_cast<DirectXVertexShader *>(vShader.Get())->GetNativeShader()), 
		m_PS(dynamic_cast<DirectXPixelShader *>(fShader.Get())->GetNativeShader()),
		m_VSReflection(nullptr), m_PSReflection(nullptr) {
		GetShaderReflection(dynamic_cast<DirectXVertexShader *>(vShader.Get())->GetShaderBytecode(),
			dynamic_cast<DirectXPixelShader *>(fShader.Get())->GetShaderBytecode());
		LOG_INFO("[DirectX] Shader Program successfully created");
	}

	DirectXShaderProgram::~DirectXShaderProgram() {}

	void prev::DirectXShaderProgram::Bind() {
		PV_PROFILE_FUNCTION();
		GetDeviceContext()->VSSetShader(m_VS.Get(), nullptr, 0u);
		GetDeviceContext()->PSSetShader(m_PS.Get(), nullptr, 0u);
	}

	pvint DirectXShaderProgram::GetUniformLocation(const pvstring & name, ShaderType type) {
		D3D11_SHADER_INPUT_BIND_DESC ibd = {};
		HRESULT hr;

		switch (type) {
		case prev::ShaderType::VERTEX_SHADER:
			hr = m_VSReflection->GetResourceBindingDescByName(name.c_str(), &ibd);
			break;
		case prev::ShaderType::FRAGMENT_SHADER:
			hr = m_PSReflection->GetResourceBindingDescByName(name.c_str(), &ibd);
			break;
		default:
		{
			LOG_ERROR("[DirectX] Inavlid shader type");
			return -1;
		}
		}

		if (FAILED(hr)) {
			LOG_ERROR("[DirectX] Unable to find shader uniform : %s", name.c_str());
			return -1;
		}
		return ibd.BindPoint;
	}

	void DirectXShaderProgram::GetShaderReflection(ComPtr<ID3DBlob> vsBytecode, ComPtr<ID3DBlob> psBytecode) {
		HRESULT hr = D3DReflect(vsBytecode->GetBufferPointer(), vsBytecode->GetBufferSize(),
			IID_ID3D11ShaderReflection, reinterpret_cast<void **>(m_VSReflection.GetAddressOf()));
		if (FAILED(hr)) {
			LOG_ERROR("[DirectX] Unable to get vertex shader reflection");
		}

		hr = D3DReflect(psBytecode->GetBufferPointer(), psBytecode->GetBufferSize(),
			IID_ID3D11ShaderReflection, reinterpret_cast<void **>(m_PSReflection.GetAddressOf()));
		if (FAILED(hr)) {
			LOG_ERROR("[DirectX] Unable to get pixel shader reflection");
		}
	}

}

#endif
