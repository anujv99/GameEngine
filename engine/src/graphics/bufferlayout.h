#pragma once

#include "../common/handledobject.h"
#include "../common/types.h"

#include <vector>

namespace prev {

	enum class DataType {
		None, Int, Int2, Int3, Int4, Float, Float2,	Float3,	Float4,	Bool
	};

	pvuint GetSizeBytes(DataType type);

	struct Entry {
		DataType Type;
		pvuint SizeBytes;
		pvuint OffsetBytes;
		pvbool Normalize;
		pvstring Name;

		pvuint GetNumElements() const;
	};

	class BufferLayout : public HandledObject<BufferLayout> {
		friend class StrongHandle<BufferLayout>;
	protected:
		BufferLayout();
		virtual ~BufferLayout() {}
	public:
		void BeginEntries(pvuint vertexSizeBytes);
		void AddEntry(DataType type, pvuint offsetBytes, pvstring semanticName, pvbool normalize = false);
		void EndEntries();

		inline pvuint GetNumEntries() const { return m_Entries.size(); }
		inline pvuint GetStride() const { return m_Stride; }

		inline std::vector<Entry>::const_iterator begin() const { return m_Entries.begin(); }
		inline std::vector<Entry>::const_iterator end() const { return m_Entries.end(); }
	public:
		static StrongHandle<BufferLayout> Create();

		std::vector<Entry> m_Entries;
		pvuint m_VertexSizeBytes;
		pvuint m_Stride;
		pvbool m_IsConstructing;
	};

}
