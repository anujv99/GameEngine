#pragma once

#include "../math/math.h"
#include "../common/handledobject.h"

namespace prev {

	class OrthoCamera : public HandledObject<OrthoCamera> {
	public:
		OrthoCamera(pvfloat left, pvfloat right, pvfloat top, pvfloat bottom, pvfloat near = -1.0f, pvfloat far = 1.0f);
		~OrthoCamera() = default;
	public:
		const Mat4 & GetProjectionView() const;
		inline bool IsMatrixChanged() const { return m_IsChanged; }
	private:
		Vec3 m_Position;
		pvfloat m_Rotation;

		Mat4 m_View;
		Mat4 m_Projection;

		mutable Mat4 m_ProjectionView;
		mutable bool m_IsChanged;
	};

}