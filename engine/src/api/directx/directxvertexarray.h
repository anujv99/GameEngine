#pragma once

#ifdef ENGINE_DIRECTX

#include "directxhelper.h"
#include "directxshader.h"

#include "graphics/vertexarray.h"

namespace prev {

	DXGI_FORMAT GetDirectXType(DataType dataType);

	class DirectXVertexArray : public VertexArray {
	public:
		DirectXVertexArray(StrongHandle<VertexShader> vShader);
		~DirectXVertexArray();
	public:
		virtual void Bind() override;
		virtual void UnBind() override;
		virtual void AddVertexBuffer(const StrongHandle<VertexBuffer> vertexBuffer) override;
	private:
		ComPtr<ID3D11InputLayout> m_InputLayout = nullptr;
		std::vector<D3D11_INPUT_ELEMENT_DESC> m_InputDescs;
		std::vector<StrongHandle<VertexBuffer>> m_VertexBuffers;

		StrongHandle<VertexShader> m_VertexShader;
		pvuint m_VBOIndex;
	};

}

#endif