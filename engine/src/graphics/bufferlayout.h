#pragma once

#include "../common/handledobject.h"
#include "../common/types.h"

#include <vector>

namespace prev {

	enum class DataType {
		None,
		Int, Int2, Int3, Int4,
		UInt, UInt2, UInt3, UInt4,
		Float, Float2,	Float3,	Float4,
		UByte, UByte2, /*UByte3,*/ UByte4,	//3 Component byte not available in DirectX11. Moreover it cause alignement problem
		Byte, Byte2, /*Byte3,*/ Byte4,		//----------------------------------------''---------------------------------------
		Bool,
	};

	pvuint GetSizeBytes(DataType type);

	struct Entry {
		pvstring Name;
		DataType Type;
		pvuint OffsetBytes;
		pvbool Normalize;

		pvuint GetNumElements() const;
	};

	class BufferLayout : public HandledObject<BufferLayout> {
		friend class StrongHandle<BufferLayout>;
	protected:
		BufferLayout();
		virtual ~BufferLayout() {}
	public:
		void BeginEntries();
		void AddEntry(DataType type, pvuint offsetBytes, pvstring semanticName, pvbool normalize = false);
		void EndEntries();

		inline pvuint GetNumEntries() const { return m_Entries.size(); }

		inline std::vector<Entry>::const_iterator begin() const { return m_Entries.begin(); }
		inline std::vector<Entry>::const_iterator end() const { return m_Entries.end(); }
	public:
		static StrongHandle<BufferLayout> Create();

		std::vector<Entry> m_Entries;
		pvuint m_VertexSizeBytes;
		pvbool m_IsConstructing;
	};

}
