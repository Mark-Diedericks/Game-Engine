#pragma once

#include "Camera.h"

namespace gebase {
	namespace graphics {

		class GE_API FPSCamera : public Camera
		{
		private:
			float m_MouseSensitivity;
			float m_Speed;
			float m_SprintSpeed;
			float m_Pitch;
			float m_Yaw;
			bool m_MouseWasGrabbed;

			math::Quaternion getOrientation() const;
			inline math::Vector3f getForwardDirection(const math::Quaternion& orientation) const { return math::Quaternion::Rotate(orientation, math::Vector3f(0.0f, 0.0f, -1.0f)); }
			inline math::Vector3f getUpDirection(const math::Quaternion& orientation) const { return math::Quaternion::Rotate(orientation, math::Vector3f(0.0f, 1.0f, 0.0f)); }
			inline math::Vector3f getRightDirection(const math::Quaternion& orientation) const { return math::Quaternion::Rotate(orientation, math::Vector3f(1.0f, 0.0f, 0.0f)); }
		public:
			FPSCamera(const math::Matrix4f& projectionMatrix);
			~FPSCamera();

			void Focus() override;
			void Update(float delta) override;
		};

	}
}