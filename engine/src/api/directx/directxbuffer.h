#pragma once

#ifdef ENGINE_DIRECTX

#include "graphics/buffer.h"

#include "directxhelper.h"

namespace prev {

	D3D11_USAGE GetDirectXType(BufferUsage usage);

	// ------------- VERTEX BUFFER -------------

	class DirectXVertexBuffer : public VertexBuffer {
	public:
		DirectXVertexBuffer(const void * data, pvsizet size, pvuint stride, BufferUsage usage);
		~DirectXVertexBuffer();
	public:
		virtual void Bind() const override;
		virtual void UnBind() const override;
		virtual void SubData(const void * data, pvsizet size, pvsizet offset) override;
		virtual void SetBufferLayout(StrongHandle<BufferLayout> layout) override;
		virtual const StrongHandle<BufferLayout> GetLayout() const override { return m_Layout; }
		virtual pvuint GetStride() const override { return m_StrideBytes; };

		void Bind(pvuint slot) const;

		inline ComPtr<ID3D11Buffer> GetBuffer() { return m_Buffer; }
	private:
		StrongHandle<BufferLayout> m_Layout;
		ComPtr<ID3D11Buffer> m_Buffer;

		pvuint m_StrideBytes;
		pvsizet m_Size;
		BufferUsage m_Usage;
	};

	// ------------- UNIFORM BUFFER -------------

	class DirectXConstantBuffer : public UniformBuffer {
	public:
		DirectXConstantBuffer(const void * data, pvsizet size, BufferUsage usage);
		~DirectXConstantBuffer();
	public:
		virtual void Bind(pvuint block, ShaderType type) const override;
		virtual void UnBind() const override;
		virtual void SubData(const void * data, pvsizet size, pvsizet offset) override;
	private:
		ComPtr<ID3D11Buffer> m_Buffer;
		pvsizet m_Size;
		BufferUsage m_Usage;
	};

}

#endif