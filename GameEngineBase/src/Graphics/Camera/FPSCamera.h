#pragma once

#include "Camera.h"

namespace gebase { namespace graphics {

	class GE_API FPSCamera : public Camera
	{
	private:
		float m_MouseSensitivity;
		float m_Speed;
		float m_SprintSpeed;
		bool m_MouseWasGrabbed;

		inline math::Quaternion getOrientation() const { return m_Rotation; }
		inline math::Vector3f getForwardDirection(const math::Quaternion& orientation) const { return orientation.getRotateForward(); }
		inline math::Vector3f getUpDirection(const math::Quaternion& orientation) const { return orientation.getRotateUp(); }
		inline math::Vector3f getRightDirection(const math::Quaternion& orientation) const { return orientation.getRotateRight(); }
	public:
		FPSCamera(const math::Matrix4f& projectionMatrix);
		~FPSCamera();

		void Focus() override;
		void Update(float delta) override;
	};

} }