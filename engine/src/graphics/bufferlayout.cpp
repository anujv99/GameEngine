#include "bufferlayout.h"

namespace prev {

    StrongHandle<BufferLayout> BufferLayout::Create() {
        return new BufferLayout();
    }

	pvuint prev::GetSizeBytes(DataType type) {
		switch (type) {
        case prev::DataType::Int:       return 4;
        case prev::DataType::Int2:      return 4 * 2;
        case prev::DataType::Int3:      return 4 * 3;
        case prev::DataType::Int4:      return 4 * 4;
        case prev::DataType::Float:     return 4;
        case prev::DataType::Float2:    return 4 * 2;
        case prev::DataType::Float3:    return 4 *3;
        case prev::DataType::Float4:    return 4 * 4;
        case prev::DataType::Bool:      return 1;
		}

        ASSERTM(false, "Inavlid data type");
        return 0;
    }

    BufferLayout::BufferLayout() : m_Entries(), m_VertexSizeBytes(0u), m_Stride(0u), m_IsConstructing(false) {}

    void BufferLayout::BeginEntries(pvuint vertexSizeBytes) {
        m_VertexSizeBytes = vertexSizeBytes;
        m_IsConstructing = true;
    }

    void BufferLayout::AddEntry(DataType type, pvuint offsetBytes, pvstring semanticName, pvbool normalize) {
        ASSERTM(m_IsConstructing, "Forgot to call BeginEntries");

        Entry entry;
        entry.Type          = type;
        entry.OffsetBytes   = offsetBytes;
        entry.Normalize     = normalize;
        entry.SizeBytes     = GetSizeBytes(entry.Type);
        entry.Name          = semanticName;
        
        if (entry.OffsetBytes + entry.SizeBytes > m_Stride) m_Stride = entry.OffsetBytes + entry.SizeBytes;

        m_Entries.push_back(entry);
    }

    void BufferLayout::EndEntries() {
        ASSERTM(m_IsConstructing, "Forgot to call BeginEntries");
        m_IsConstructing = false;
    }

    pvuint Entry::GetNumElements() const {
        switch (Type) {
        case prev::DataType::Int:       return 1;
        case prev::DataType::Int2:      return 2;
        case prev::DataType::Int3:      return 3;
        case prev::DataType::Int4:      return 4;
        case prev::DataType::Float:     return 1;
        case prev::DataType::Float2:    return 2;
        case prev::DataType::Float3:    return 3;
        case prev::DataType::Float4:    return 4;
        case prev::DataType::Bool:      return 1;
        }

        ASSERTM(false, "Inavlid data type");
        return 0;
    }

}