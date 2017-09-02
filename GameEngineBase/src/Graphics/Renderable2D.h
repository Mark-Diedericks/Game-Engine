#pragma once

#include "Common.h"
#include "Texture/Texture.h"
#include "Buffer/VertexBuffer.h"
#include "Buffer/IndexBuffer.h"
#include "Shader/Shader.h"
#include "Math/Maths.h"
#include "IRenderableBase.h"

namespace gebase { namespace graphics {

	class Renderer2D;

	struct GE_API VertexData
	{
		math::Vector3f vertex;
		math::Vector2f tex_uv;
		math::Vector2f mask_uv;
		float tid;
		float mid;
		uint color;
	};

#define RENDERER_VERTEX_SIZE sizeof(VertexData)

	class GE_API Renderable2D : public IRenderableBase
	{
	protected:
		math::Rectangle m_Bounds;
		uint m_Color;
		std::vector<math::Vector2f> m_UVs;
		Texture* m_Texture;
		bool m_Visible;

		Renderable2D();
		Renderable2D(const math::Vector2f& position, const math::Vector2f& size, uint color);
	public:
		virtual ~Renderable2D();

		virtual void Submit(Renderer2D* renderer) const;
		virtual bool EmployRenderAPI(RenderAPI api) = 0;
		
		inline const math::Vector2f& getPosition() const { return m_Bounds.position; }
		inline const math::Vector2f& getSize() const { return m_Bounds.size; }
		
		inline const math::Rectangle& getBounds() const { return m_Bounds; }
		inline		 math::Rectangle& getBounds()		{ return m_Bounds; }

		inline math::AABB getAABB() const { return m_Bounds; }

		inline const uint getColor() const { return m_Color; }
		inline void setColor(uint color) { m_Color = color; }
		void setColor(const math::Vector4f& color);

		inline Texture* getTexture() const { return m_Texture; }
		inline const std::vector<math::Vector2f>& getUVs() const { return m_UVs; }
		
		inline bool isVisible() const { return m_Visible; }

		inline void setPosition(const math::Vector2f& position) { m_Bounds.position = position; }
		inline void setSize(const math::Vector2f& size) { m_Bounds.size = size; }

		static const std::vector<math::Vector2f>& getDefaultUVs();

	};

} }