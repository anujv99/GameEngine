#pragma once

#include "../common/types.h"

#include <stack>
#include <prevmath/matrix/mat4.hpp>

namespace prev {

	class MatrixStack {
	public:
		MatrixStack() : m_IsChanged(true) { m_Stack.push(prevmath::Mat4()); }
		~MatrixStack() {}
	public:
		inline void Push(const prevmath::Mat4 & _mat) { m_Stack.push(_mat); m_IsChanged = true; }
		inline void Pop() { m_Stack.pop(); m_IsChanged = true; }
		inline prevmath::Mat4 Get() const { m_IsChanged = false; return m_Stack.top(); }

		inline pvsizet Size() const { return m_Stack.size(); }
		inline pvbool IsChanged() const { return m_IsChanged; }
	private:
		std::stack<prevmath::Mat4> m_Stack;
		mutable pvbool m_IsChanged;
	};

}
