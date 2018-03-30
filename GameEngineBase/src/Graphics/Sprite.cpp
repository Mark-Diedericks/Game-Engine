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

	void Sprite::setUV(const std::vector<math::Vector2f>& UVs)
	{
		m_UVs = UVs;
	}

	bool Sprite::EmployRenderAPIShader(RenderAPI api)
	{
		return true;
	}

	bool Sprite::EmployRenderAPITexture2D(RenderAPI api)
	{
		if (m_Texture)
			m_Texture = Texture::ConvertRenderAPI(api, m_Texture);

		return true;
	}

	bool Sprite::EmployRenderAPITextureCube(RenderAPI api)
	{
		return true;
	}

	bool Sprite::EmployRenderAPITextureDepth(RenderAPI api)
	{
		return true;
	}

	bool Sprite::EmployRenderAPIFramebuffer2D(RenderAPI api)
	{
		return true;
	}

	bool Sprite::EmployRenderAPIFramebufferDepth(RenderAPI api)
	{
		return true;
	}

	bool Sprite::EmployRenderAPIIndexBuffer(RenderAPI api)
	{
		return true;
	}

	bool Sprite::EmployRenderAPIVertexArray(RenderAPI api)
	{
		return true;
	}


} }