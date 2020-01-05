#pragma once

#include <cstring>
#include <string>
#include <ostream>
#include <cassert>
#include <cstdint>

namespace prev {

	class PrevString {
	public:
		PrevString() = default;
		PrevString(const char * _data) : m_String(const_cast<char *>(_data)) {}

		PrevString(const PrevString & _str) : 
			m_String(_str.m_String), m_Ref(_str.m_Ref), m_Size(_str.m_Size), m_AllocatedBufferSize(_str.m_AllocatedBufferSize) {
			if (m_Ref != nullptr) *m_Ref += 1u;
		}

		PrevString(const std::string & _str) : m_String(nullptr), m_Size(_str.size()) {
			Allocate(m_Size);
			memcpy_s(reinterpret_cast<void *>(m_String), m_Size, reinterpret_cast<const void *>(_str.data()), _str.size());
		}

		~PrevString() {
			Deallocate();
		}
	public:
		inline const char * c_str() const {
			return m_String;
		}

		inline char * data() {
			return m_String;
		}

		inline size_t size() const {
			if (m_String == nullptr) {
				return 0u;
			}
			if (m_Size == 0u) {
				m_Size = strlen(m_String);
			}
			return m_Size;
		}

		inline size_t length() const {
			return size();
		}

		inline void clear() {
			Deallocate();
		}

		inline void resize(size_t newsize) {
			if (m_Ref != nullptr && *m_Ref == 1u && m_AllocatedBufferSize > newsize) {
				m_Size = newsize;
				m_String[m_Size] = '\0';
				return;
			}
			Deallocate();
			Allocate(newsize);
		}

		bool operator==(const char * _str) {
			return strcmp(m_String, _str) == 0;
		}

		bool operator==(const PrevString & _str) {
			return strcmp(m_String, _str.m_String) == 0;
		}

		char & operator[](size_t index) {
			assert(index < size());
			return m_String[index];
		}

		const char & operator[](size_t index) const {
			assert(index < size());
			return m_String[index];
		}

		void operator=(const char * str) {
			if (str == nullptr) {
				Deallocate();
				return;
			}
			size_t size = strlen(str);
			resize(size);
			memcpy_s(m_String, m_Size, str, size);
		}

		void operator=(const PrevString & _str) {
			Deallocate();
			m_String = _str.m_String;
			m_Size = _str.m_Size;
			m_AllocatedBufferSize = _str.m_AllocatedBufferSize;
			m_Ref = _str.m_Ref;
			if (m_Ref != nullptr) *m_Ref += 1u;
		}

		void operator=(const std::string & _str) {
			*this = _str.c_str();
		}

	private:
		void Deallocate() {
			if (m_Ref != nullptr) {
				*m_Ref -= 1u;
				if (*m_Ref == 0) {
					delete[] m_String;
					delete m_Ref;
				}
			}
			m_String = nullptr;
			m_Ref = nullptr;
			m_Size = 0u;
			m_AllocatedBufferSize = 0u;
		}

		void Allocate(size_t size) {
			m_String = new char[size + 1];
			m_String[size] = '\0';
			m_Size = size;
			m_AllocatedBufferSize = size + 1;
			m_Ref = new unsigned int;
			*m_Ref = 1u;
		}

	private:
		mutable char * m_String					= nullptr;
		mutable unsigned int * m_Ref			= nullptr;
		mutable size_t m_AllocatedBufferSize	= 0u;
		mutable size_t m_Size					= 0u;
	};

	inline std::ostream & operator<<(std::ostream & os, const PrevString & str) {
		os << str.c_str();
		return os;
	}

	inline uint32_t HashString(const PrevString & str) {
		uint32_t hash = 0u;
		for (size_t i = 0; i < str.size(); i++) {
			hash = 65599u * hash + static_cast<uint32_t>(str[i]);
		}
		return hash ^ (hash >> 16);
	}

	inline uint64_t HashUint32Pair(uint32_t val1, uint32_t val2) {
		return static_cast<uint64_t>(val1) | (static_cast<uint64_t>(val2) << 32);
	}

	inline uint64_t HashStringPair(const PrevString & str1, const PrevString & str2) {
		uint32_t hash1 = HashString(str1);
		uint32_t hash2 = HashString(str2);

		return HashUint32Pair(hash1, hash2);
	}

}
