#pragma once

#ifdef ENGINE_DIRECTX

#include <d3dcompiler.h>

#include "directxhelper.h"

#include "graphics/shader.h"

namespace prev {

	// ------------- VERTEX SHADER -------------

	class DirectXVertexShader : public VertexShader {
	public:
		DirectXVertexShader(const pvstring & source);
		~DirectXVertexShader();

		void Bind();

		inline ComPtr<ID3DBlob> GetShaderBytecode() { return m_ShaderBytecode; }
		inline ComPtr<ID3D11VertexShader> GetNativeShader() { return m_VertexShader; }
	private:
		ComPtr<ID3D11VertexShader> m_VertexShader;
		ComPtr<ID3DBlob> m_ShaderBytecode;
	};

	// ------------- FRAGMENT SHADER -------------

	class DirectXPixelShader : public FragmentShader {
	public:
		DirectXPixelShader(const pvstring & source);
		~DirectXPixelShader();

		void Bind();

		inline ComPtr<ID3DBlob> GetShaderBytecode() { return m_ShaderBytecode; }
		inline ComPtr<ID3D11PixelShader> GetNativeShader() { return m_PixelShader; }
	private:
		ComPtr<ID3D11PixelShader> m_PixelShader;
		ComPtr<ID3DBlob> m_ShaderBytecode;
	};

	// ------------- SHADER PROGRAM -------------

	class DirectXShaderProgram : public ShaderProgram {
	public:
		DirectXShaderProgram(StrongHandle<VertexShader> vShader, StrongHandle<FragmentShader> fShader);
		~DirectXShaderProgram();

		virtual void Bind() override;
		virtual void UnBind() override {};
		virtual pvint GetUniformLocation(const pvstring & name, ShaderType type) override;
	private:
		void GetShaderReflection(ComPtr<ID3DBlob> vsBytecode, ComPtr<ID3DBlob> psBytecode);
	private:
		ComPtr<ID3D11VertexShader> m_VS;
		ComPtr<ID3D11PixelShader> m_PS;
		ComPtr<ID3D11ShaderReflection> m_VSReflection;
		ComPtr<ID3D11ShaderReflection> m_PSReflection;
	};

}

#endif