#pragma once

#include "../common/singleton.h"

#include "matrixstack.h"

namespace prev {

	class MVPStack : public Singleton<MVPStack> {
		friend class Singleton<MVPStack>;
	private:
		MVPStack() { m_Cache = m_Projection.Get() * m_View.Get() * m_Model.Get(); }
		~MVPStack() {}
	public:
		inline pvbool IsChanged() const { return (m_Model.IsChanged() || m_View.IsChanged() || m_Projection.IsChanged()); }
		prevmath::Mat4 GetMVP() const { if (IsChanged()) { RecalculateMVP(); } return m_Cache; }

		MatrixStack & Model() { return m_Model; };
		MatrixStack & View() { return m_View; };
		MatrixStack & Projection() { return m_Projection; };
	private:
		inline void RecalculateMVP() const {
			m_Cache = m_Projection.Get() * m_View.Get() * m_Model.Get();
		}
	private:
		mutable MatrixStack m_Model;
		mutable MatrixStack m_View;
		mutable MatrixStack m_Projection;

		mutable prevmath::Mat4 m_Cache;
	};

}