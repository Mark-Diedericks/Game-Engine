#pragma once

#include "Light.h"

namespace gebase { namespace graphics {

	class GE_API LightSetup
	{
	private:
		std::vector<Light*> m_Lights;
	public:
		LightSetup();
		~LightSetup();

		Light* Add(Light* light);
		void Remove(Light* light);

		inline const std::vector<Light*>& getLights() const { return m_Lights; }
	};

} }