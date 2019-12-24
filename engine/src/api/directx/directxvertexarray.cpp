#include "directxvertexarray.h"

#ifdef ENGINE_DIRECTX

namespace prev {

	extern pvuint NUMBER_OF_DRAW_CALLS;

	DXGI_FORMAT GetDirectXType(DataType dataType, pvbool normalize) {
		if (normalize) {
			switch (dataType) {
			case prev::DataType::Int:
			case prev::DataType::Int2:
			case prev::DataType::Int3:
			case prev::DataType::Int4:
			case prev::DataType::UInt:
			case prev::DataType::UInt2:
			case prev::DataType::UInt3:
			case prev::DataType::UInt4:
			case prev::DataType::Float:
			case prev::DataType::Float2:
			case prev::DataType::Float3:
			case prev::DataType::Float4:	ASSERTM(false, "DirectX dosen't support normalized 32-bit values");
			case prev::DataType::Bool:		return DXGI_FORMAT_R8_UNORM;
			case prev::DataType::UByte:		return DXGI_FORMAT_R8_UNORM;
			case prev::DataType::UByte2:	return DXGI_FORMAT_R8G8_UNORM;
			case prev::DataType::UByte4:	return DXGI_FORMAT_R8G8B8A8_UNORM;
			case prev::DataType::Byte:		return DXGI_FORMAT_R8_SNORM;
			case prev::DataType::Byte2:		return DXGI_FORMAT_R8G8_SNORM;
			case prev::DataType::Byte4:		return DXGI_FORMAT_R8G8B8A8_SNORM;
			}
		} else {
			switch (dataType) {
			case prev::DataType::Int:		return DXGI_FORMAT_R32_SINT;
			case prev::DataType::Int2:		return DXGI_FORMAT_R32G32_SINT;
			case prev::DataType::Int3:		return DXGI_FORMAT_R32G32B32_SINT;
			case prev::DataType::Int4:		return DXGI_FORMAT_R32G32B32A32_SINT;
			case prev::DataType::UInt:		return DXGI_FORMAT_R32_UINT;
			case prev::DataType::UInt2:		return DXGI_FORMAT_R32G32_UINT;
			case prev::DataType::UInt3:		return DXGI_FORMAT_R32G32B32_UINT;
			case prev::DataType::UInt4:		return DXGI_FORMAT_R32G32B32A32_UINT;
			case prev::DataType::Float:		return DXGI_FORMAT_R32_FLOAT;
			case prev::DataType::Float2:	return DXGI_FORMAT_R32G32_FLOAT;
			case prev::DataType::Float3:	return DXGI_FORMAT_R32G32B32_FLOAT;
			case prev::DataType::Float4:	return DXGI_FORMAT_R32G32B32A32_FLOAT;
			case prev::DataType::Bool:		return DXGI_FORMAT_R8_UNORM;
			case prev::DataType::UByte:		return DXGI_FORMAT_R8_UINT;
			case prev::DataType::UByte2:	return DXGI_FORMAT_R8G8_UINT;
			case prev::DataType::UByte4:	return DXGI_FORMAT_R8G8B8A8_UINT;
			case prev::DataType::Byte:		return DXGI_FORMAT_R8_SINT;
			case prev::DataType::Byte2:		return DXGI_FORMAT_R8G8_SINT;
			case prev::DataType::Byte4:		return DXGI_FORMAT_R8G8B8A8_SINT;
			}
		}

		ASSERTM(false, "Inavlid data type");
		return DXGI_FORMAT_UNKNOWN;
	}

	DEFINE_OBJECT_DIRECTX(VertexArray, StrongHandle<VertexShader> vShader) {
		return dynamic_cast<VertexArray *>(new DirectXVertexArray(vShader));
	}

	DirectXVertexArray::DirectXVertexArray(StrongHandle<VertexShader> vShader) : m_VBOIndex(0u), m_VertexShader(vShader) {}

	DirectXVertexArray::~DirectXVertexArray() {}

	void DirectXVertexArray::Bind() {
		PV_PROFILE_FUNCTION();
		if (m_InputLayout == nullptr) {
			DirectXVertexShader * vShader = dynamic_cast<DirectXVertexShader *>(m_VertexShader.Get());
			ComPtr<ID3DBlob> shaderBytecode = vShader->GetShaderBytecode();

			ASSERT(shaderBytecode != nullptr);

			HRESULT hr = GetDevice()->CreateInputLayout(
				m_InputDescs.data(), (UINT)m_InputDescs.size(),
				shaderBytecode->GetBufferPointer(),
				shaderBytecode->GetBufferSize(),
				m_InputLayout.GetAddressOf()
			);

			if (FAILED(hr)) {
				ASSERT(false, "Failed to create input layout");
				return;
			}
		}

		GetDeviceContext()->IASetInputLayout(m_InputLayout.Get());
		for (pvsizet i = 0; i < m_VertexBuffers.size(); i++) {
			m_VertexBuffers[i]->Bind(i);
		}
	}

	void DirectXVertexArray::UnBind() {}

	void DirectXVertexArray::AddVertexBuffer(const StrongHandle<VertexBuffer> vertexBuffer) {
		PV_PROFILE_FUNCTION();
		ASSERTM(vertexBuffer->GetLayout() != nullptr && vertexBuffer->GetLayout()->GetNumEntries() > 0, "Vertex Buffer has inavlid layout");

		const StrongHandle<BufferLayout> & layout = vertexBuffer->GetLayout();
		for (const auto & entry : *layout) {
			m_InputDescs.emplace_back();

			m_InputDescs.back().SemanticName			= entry.Name.c_str();
			m_InputDescs.back().SemanticIndex			= 0u;
			m_InputDescs.back().Format					= GetDirectXType(entry.Type, entry.Normalize);
			m_InputDescs.back().InputSlot				= m_VBOIndex;
			m_InputDescs.back().AlignedByteOffset		= entry.OffsetBytes;
			m_InputDescs.back().InputSlotClass			= D3D11_INPUT_PER_VERTEX_DATA;
			m_InputDescs.back().InstanceDataStepRate	= 0u;
		}

		m_VBOIndex++;

		m_VertexBuffers.push_back(dynamic_cast<DirectXVertexBuffer *>(vertexBuffer.Get()));
	}

	void DirectXVertexArray::Draw(pvuint numElements) {
		PV_PROFILE_FUNCTION();
		GetDeviceContext()->Draw(numElements, 0);
		NUMBER_OF_DRAW_CALLS++;
	}

}

#endif