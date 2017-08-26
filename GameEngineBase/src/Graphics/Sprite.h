#pragma once

#include "Renderable2D.h"
#include "Texture/Texture2D.h"

namespace gebase { namespace graphics {

	class GE_API Sprite : public Renderable2D
	{
	protected:
		Sprite();
	public:
		Sprite(Texture2D* texture);
		Sprite(float x, float y, Texture2D* texture);
		Sprite(float x, float y, float width, float height, uint color);
		Sprite(float x, float y, float width, float height, const math::Vector4f& color);
		Sprite(float x, float y, float width, float height, Texture* texture);

		void setUV(const std::vector<math::Vector2f>& UVs);
		
		inline void setTexture(Texture2D* texture) { m_Texture = texture; }
	};

} }