#pragma once

#include "Common.h"
#include "Math/Maths.h"

namespace gebase { namespace graphics {

	class GE_API Camera
	{
	protected:
		math::Matrix4f m_ProjectionMatrix;
		math::Matrix4f m_ViewMatrix;

		math::Vector3f m_Position;
		math::Vector3f m_FocalPoint;
		math::Quaternion m_Rotation;
	public:
		Camera(const math::Matrix4f& projectionMatrix);

		virtual void Focus();
		virtual void Update(float delta);

		inline const math::Vector3f& getPosition() const { return m_Position; }
		inline void setPosition(const math::Vector3f& position) { m_Position = position; }

		inline const math::Vector3f& getFocalPoint() const { return m_FocalPoint; }

		inline const math::Quaternion& getRotation() const { return m_Rotation; }
		inline void setRotation(const math::Quaternion& rotation) { m_Rotation = rotation; }

		inline const math::Matrix4f& getProjectionMatrix() const { return m_ProjectionMatrix; }
		inline void setProjectionMatrix(const math::Matrix4f& projection) { m_ProjectionMatrix = projection; }

		inline const math::Matrix4f& getViewMatrix() const { return m_ViewMatrix; }

		inline void Translate(const math::Vector3f& translation) { m_Position += translation; }
		inline void Translate(const math::Vector3f& direction, float distance) { m_Position += (direction * distance); }
		inline void Translate(float x, float y, float z) { m_Position += math::Vector3f(x, y, z); }

		inline void Rotate(const math::Quaternion& rotation) { m_Rotation *= rotation; }
		inline void Rotate(const math::Vector3f& rotation) { m_Rotation.rotate(rotation); }
		inline void Rotate(float x, float y, float z) { m_Rotation.rotate(math::Vector3f(x, y, z)); }

		inline void AimAt(const math::Vector3f& point, const math::Vector3f& up) { m_Rotation = getRotationToPoint(point, up); }
		inline math::Quaternion& getRotationToPoint(const math::Vector3f& point, const math::Vector3f& up) { return math::Quaternion(*math::Matrix4f::initRotation((point - m_Position).normalize(), up)); }
	};

} }