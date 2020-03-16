#pragma once

#include "../common/types.h"

namespace prev {

	class Path {
	public:
		static inline pvstring ToResFolderPath(const pvstring & path) { return (m_ResFolderPath + path); }
		static inline void SetResFolderPath(const pvstring & path) { m_ResFolderPath = path; }
	private:
		static pvstring m_ResFolderPath;
	};

}