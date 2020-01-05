#include "orthocamera.h"

namespace prev {

	OrthoCamera::OrthoCamera(pvfloat left, pvfloat right, pvfloat top, pvfloat bottom, pvfloat near, pvfloat far) :
		m_Rotation(0.0f), m_IsChanged(true) {
		m_Projection = Mat4::Ortho(left, right, bottom, top, near, far);
	}

	const Mat4 & OrthoCamera::GetProjectionView() const {
		if (m_IsChanged) {
			m_ProjectionView = m_Projection * m_View;
			m_IsChanged = false;
		}
		return m_ProjectionView;
	}

}