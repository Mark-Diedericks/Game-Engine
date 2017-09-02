#include "ge.h"
#include "Common.h"
#include "Renderable2D.h"
#include "Renderer/Renderer2D.h"

namespace gebase { namespace graphics {

	Renderable2D::Renderable2D() : IRenderableBase(), m_Texture(nullptr)
	{
		m_UVs = getDefaultUVs();
	}

	Renderable2D::Renderable2D(const math::Vector2f& position, const math::Vector2f& size, uint color) : IRenderableBase(), m_Bounds(position, size), m_Color(color), m_Texture(nullptr), m_Visible(true)
	{
		m_UVs = getDefaultUVs();
	}

	Renderable2D::~Renderable2D()
	{

	}

	void Renderable2D::Submit(Renderer2D* renderer) const
	{
		renderer->Submit(this);
	}

	void Renderable2D::setColor(const math::Vector4f& color)
	{
		uint r = (uint)(color.x * 255.0f);
		uint g = (uint)(color.y * 255.0f);
		uint b = (uint)(color.z * 255.0f);
		uint a = (uint)(color.w * 255.0f);

		m_Color = a << 24 | b << 16 | g << 8 | r << 0;
	}


	const std::vector<math::Vector2f>& Renderable2D::getDefaultUVs()
	{
		static std::vector<math::Vector2f> uvs;

		if (!uvs.size())
		{
			uvs.push_back(math::Vector2f(0, 1));
			uvs.push_back(math::Vector2f(1, 1));
			uvs.push_back(math::Vector2f(1, 0));
			uvs.push_back(math::Vector2f(0, 0));
		}

		return uvs;
	}
} }