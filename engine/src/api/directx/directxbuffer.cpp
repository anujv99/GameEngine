#include "directxbuffer.h"

#ifdef ENGINE_DIRECTX

namespace prev {

	// ------------- VERTEX BUFFER -------------

	DEFINE_OBJECT_DIRECTX(VertexBuffer, const void * data, pvsizet size, pvuint stride, BufferUsage usage) {
		return dynamic_cast<VertexBuffer *>(new DirectXVertexBuffer(data, size, stride, usage));
	}

	DirectXVertexBuffer::DirectXVertexBuffer(const void * data, pvsizet size, pvuint stride, BufferUsage usage) :
		m_StrideBytes(stride), m_Usage(usage), m_Size(size) {
		PV_PROFILE_FUNCTION();

		ASSERT(size > 0);

		UINT cpuAccess = 0u;

		switch (usage) {
		case prev::BufferUsage::USAGE_STATIC:
			cpuAccess = 0u;
			break;
		case prev::BufferUsage::USAGE_DYNAMIC:
			cpuAccess = D3D11_CPU_ACCESS_WRITE;
			break;
		case prev::BufferUsage::USAGE_STREAM:
			cpuAccess = D3D11_CPU_ACCESS_WRITE;
			break;
		default:
			break;
		}

		if (usage == BufferUsage::USAGE_STATIC) {
			ASSERT(data);
		}

		D3D11_BUFFER_DESC vbd;
		vbd.ByteWidth				= size;
		vbd.BindFlags				= D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags			= cpuAccess;
		vbd.MiscFlags				= 0u;
		vbd.StructureByteStride		= stride;
		vbd.Usage					= GetDirectXType(usage);

		if (data != nullptr) {
			D3D11_SUBRESOURCE_DATA vbsd;
			vbsd.pSysMem			= data;
			vbsd.SysMemPitch		= 0;
			vbsd.SysMemSlicePitch	= 0;

			ASSERTM(GetDevice()->CreateBuffer(&vbd, &vbsd, m_Buffer.GetAddressOf()) == S_OK, "Failed to create DirectX vertex buffer");
		} else {
			ASSERTM(GetDevice()->CreateBuffer(&vbd, nullptr, m_Buffer.GetAddressOf()) == S_OK, "Failed to create DirectX vertex buffer");
		}

	}

	DirectXVertexBuffer::~DirectXVertexBuffer() {
	
	}

	void DirectXVertexBuffer::Bind() const {
		PV_PROFILE_FUNCTION();
		const static UINT OFFSET = 0;
		GetDeviceContext()->IASetVertexBuffers(0, 1, m_Buffer.GetAddressOf(), (UINT *)&m_StrideBytes, &OFFSET);
	}

	void DirectXVertexBuffer::Bind(pvuint slot) const {
		PV_PROFILE_FUNCTION();
		const static UINT OFFSET = 0;
		GetDeviceContext()->IASetVertexBuffers(slot, 1, m_Buffer.GetAddressOf(), (UINT *)&m_StrideBytes, &OFFSET);
	}

	void DirectXVertexBuffer::UnBind() const {
		PV_PROFILE_FUNCTION();
		static ComPtr<ID3D11Buffer> BUFFER;
		const static UINT OFFSET = 0;
		GetDeviceContext()->IASetVertexBuffers(0, 1, BUFFER.GetAddressOf(), (UINT *)&m_StrideBytes, &OFFSET);
	}

	void DirectXVertexBuffer::SubData(const void * data, pvsizet size, pvsizet offset) {
		PV_PROFILE_FUNCTION();
		ASSERT(data);
		ASSERTM(m_Usage != BufferUsage::USAGE_STATIC, "Cannot use SubData on buffer with Static usage");

		D3D11_MAPPED_SUBRESOURCE msr;

		HRESULT hr = GetDeviceContext()->Map(m_Buffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr);
		ASSERTM(hr == S_OK, "[DirectX] Unable to map vertex buffer for writing");
		memcpy_s(msr.pData, static_cast<rsize_t>(m_Size), data, static_cast<rsize_t>(size));
		GetDeviceContext()->Unmap(m_Buffer.Get(), 0u);
	}

	void DirectXVertexBuffer::SetBufferLayout(StrongHandle<BufferLayout> layout) {
		if (layout == nullptr) {
			LOG_ERROR("[DirectX] Inavlid buffer layout");
			return;
		}
		m_Layout = layout;
	}

	D3D11_USAGE GetDirectXType(BufferUsage usage) {
		switch (usage) {
		case prev::BufferUsage::USAGE_STATIC:
			return D3D11_USAGE_IMMUTABLE;
		case prev::BufferUsage::USAGE_DYNAMIC:
			return D3D11_USAGE_DYNAMIC;
		case prev::BufferUsage::USAGE_STREAM:
			return D3D11_USAGE_DYNAMIC;
		default:
			return D3D11_USAGE_DEFAULT;
		}
	}

	// ------------- UNIFORM BUFFER -------------

	DEFINE_OBJECT_DIRECTX(UniformBuffer, const void * data, pvsizet size, BufferUsage usage) {
		return dynamic_cast<UniformBuffer *>(new DirectXConstantBuffer(data, size, usage));
	}

	DirectXConstantBuffer::DirectXConstantBuffer(const void * data, pvsizet size, BufferUsage usage) :
		m_Usage(usage), m_Buffer(nullptr), m_Size(0) {
		PV_PROFILE_FUNCTION();

		ASSERT(size > 0);

		if (size % 16 != 0) {
			size += 16 - (size % 16);
		}

		m_Size = size;

		UINT cpuAccess = 0u;

		switch (usage) {
		case prev::BufferUsage::USAGE_STATIC:
			cpuAccess = 0u;
			break;
		case prev::BufferUsage::USAGE_DYNAMIC:
			cpuAccess = D3D11_CPU_ACCESS_WRITE;
			break;
		case prev::BufferUsage::USAGE_STREAM:
			cpuAccess = D3D11_CPU_ACCESS_WRITE;
			break;
		default:
			break;
		}

		if (usage == BufferUsage::USAGE_STATIC) {
			ASSERT(data);
		}

		D3D11_BUFFER_DESC vbd;
		vbd.ByteWidth				= size;
		vbd.BindFlags				= D3D11_BIND_CONSTANT_BUFFER;
		vbd.CPUAccessFlags			= cpuAccess;
		vbd.MiscFlags				= 0u;
		vbd.Usage					= GetDirectXType(usage);

		if (data != nullptr) {
			D3D11_SUBRESOURCE_DATA vbsd;
			vbsd.pSysMem			= data;
			vbsd.SysMemPitch		= 0;
			vbsd.SysMemSlicePitch	= 0;

			ASSERTM(GetDevice()->CreateBuffer(&vbd, &vbsd, m_Buffer.GetAddressOf()) == S_OK, "[DirectX] Failed to create DirectX vertex buffer");
		} else {
			ASSERTM(GetDevice()->CreateBuffer(&vbd, nullptr, m_Buffer.GetAddressOf()) == S_OK, "[DirectX] Failed to create DirectX vertex buffer");
		}
	}

	DirectXConstantBuffer::~DirectXConstantBuffer() {}

	void DirectXConstantBuffer::Bind(pvuint block, ShaderType type) const {
		PV_PROFILE_FUNCTION();
		switch (type) {
		case prev::ShaderType::VERTEX_SHADER:
			GetDeviceContext()->VSSetConstantBuffers(block, 1u, m_Buffer.GetAddressOf());
			break;
		case prev::ShaderType::FRAGMENT_SHADER:
			GetDeviceContext()->PSSetConstantBuffers(block, 1u, m_Buffer.GetAddressOf());
			break;
		default:
		{
			ASSERT(false);
		}
		}
	}

	void DirectXConstantBuffer::UnBind() const {}

	void DirectXConstantBuffer::SubData(const void * data, pvsizet size, pvsizet offset) {
		PV_PROFILE_FUNCTION();
		ASSERT(data);
		ASSERT(size + offset <= m_Size);
		ASSERTM(m_Usage != BufferUsage::USAGE_STATIC, "Cannot use SubData on buffer with Static usage");

		D3D11_MAPPED_SUBRESOURCE msr;

		GetDeviceContext()->Map(m_Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		memcpy_s(reinterpret_cast<void *>((reinterpret_cast<pvbyte *>(msr.pData) + offset)), m_Size, data, size);
		GetDeviceContext()->Unmap(m_Buffer.Get(), 0);
	}

}

#endif
