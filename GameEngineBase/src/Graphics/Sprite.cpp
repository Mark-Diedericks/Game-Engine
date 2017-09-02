#include "ge.h"
#include "Sprite.h"

namespace gebase { namespace graphics {

	Sprite::Sprite() : Renderable2D()
	{

	}

	Sprite::Sprite(Texture2D* texture) : Renderable2D(math::Vector3f(0.0f, 0.0f, 0.0f), math::Vector2f((float)texture->getWidth(), (float)texture->getHeight()), 0xffffffff)
	{
		m_Texture = texture;
	}

	Sprite::Sprite(float x, float y, Texture2D* texture) : Renderable2D(math::Vector3f(x, y, 0.0f), math::Vector2f((float)texture->getWidth(), (float)texture->getHeight()), 0xffffffff)
	{
		m_Texture = texture;
	}

	Sprite::Sprite(float x, float y, float width, float height, uint color) : Renderable2D(math::Vector3f(x, y, 0.0f), math::Vector2f(width, height), color)
	{
		
	}

	Sprite::Sprite(float x, float y, float width, float height, const math::Vector4f& color) : Renderable2D(math::Vector3f(x, y, 0.0f), math::Vector2f(width, height), 0xffffffff)
	{
		setColor(color);
	}

	Sprite::Sprite(float x, float y, float width, float height, Texture* texture) : Renderable2D(math::Vector3f(x, y, 0.0f), math::Vector2f(width, height), 0xffffffff)
	{
		m_Texture = texture;
	}

	bool Sprite::PreEmployRenderAPI()
	{
		return true;
	}

	bool Sprite::EmployRenderAPI(RenderAPI api)
	{
		if (m_Texture)
			if (!m_Texture->EmployRenderAPI(api))
				return false;

		return true;
	}


	void Sprite::setUV(const std::vector<math::Vector2f>& UVs)
	{
		m_UVs = UVs;
	}
} }