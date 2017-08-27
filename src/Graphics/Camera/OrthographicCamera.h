#pragma once

#include "Camera.h"

namespace gebase { namespace graphics {

	class GE_API OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(const math::Matrix4f& projectionMatrix);
		~OrthographicCamera();

		void Focus() override;
		void Update(float delta) override;
	};

} }