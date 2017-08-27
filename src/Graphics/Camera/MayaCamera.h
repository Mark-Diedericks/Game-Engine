#pragma once

#include "Common.h"
#include "Camera.h"

namespace gebase { namespace graphics {

	class GE_API MayaCamera : public Camera
	{
	private:
		bool m_Panning;
		bool m_Rotating;

		math::Vector2f m_InitialMousePosition;
		math::Vector3f m_InitialFocalPoint;
		math::Quaternion m_IntialRotatoin;

		float m_Distance;
		float m_PanSpeed;
		float m_RotationSpeed;
		float m_ZoomSpeed;

		inline math::Quaternion getOrientation() const { return m_Rotation; }
		inline math::Vector3f getForwardDirection(const math::Quaternion& orientation) const { return orientation.getRotateForward(); }
		inline math::Vector3f getUpDirection(const math::Quaternion& orientation) const { return orientation.getRotateUp(); }
		inline math::Vector3f getRightDirection(const math::Quaternion& orientation) const { return orientation.getRotateRight(); }
		inline math::Vector3f calculatePosition() { return m_FocalPoint - getForwardDirection(m_Rotation) * m_Distance; }

		void MousePan(const math::Vector2f& delta, float tDelta);
		void MouseRotate(const math::Vector2f& delta, float tDelta);
		void MouseZoom(float delta, float tDelta);
	public:
		MayaCamera(const math::Matrix4f& projectionMatrix);

		void Focus() override;
		void Update(float delta) override;

		inline float getDistance() const { return m_Distance; }
		inline void setDistance(float distance) { m_Distance = distance; }
	};

} }