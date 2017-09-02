#pragma once

#include "Common.h"
#include "Graphics/Buffer/VertexArray.h"
#include "Graphics/Buffer/IndexBuffer.h"
#include "Graphics/Material/Material.h"
#include "Graphics/IRenderable.h"

namespace gebase { namespace graphics {

	class Renderer3D;

	struct GE_API Vertex
	{
		math::Vector3f position;
		math::Vector2f uv;
		math::Vector3f normal;
		math::Vector3f binormal;
		math::Vector3f tangent;
	};

	class GE_API Mesh : public IRenderable
	{
	private:
		VertexArray* m_VertexArray;
		IndexBuffer* m_IndexBuffer;
		MaterialInstance* m_MaterialInstance;

#ifdef GE_DEBUG
		Vertex* m_DebugVertexData;
		uint m_DebugVertexDataCount;
		bool m_DebugDraw;
#endif
	public:
		Mesh(VertexArray* vertexArray, IndexBuffer* indexBuffer, MaterialInstance* materialInstance);
		Mesh(const Mesh* mesh);
		~Mesh();

		void Render(Renderer3D& renderer) override;

		bool PreEmployRenderAPI() override;
		bool EmployRenderAPI(RenderAPI api) override;

		inline void setMaterial(MaterialInstance* materialInstance) { m_MaterialInstance = materialInstance; }
		inline MaterialInstance* getMaterialInstance() const { return m_MaterialInstance; }

#ifdef GE_DEBUG
		void DebugRender(const math::Matrix4f& transform);

		inline void setDebugData(Vertex* vertices, uint count) { m_DebugVertexData = vertices; m_DebugVertexDataCount = count; }
		inline uint getDebuData(Vertex*& vertices) const { vertices = m_DebugVertexData; return m_DebugVertexDataCount; }

		inline void setDebugDraw(bool enabled) { m_DebugDraw = enabled; }
#endif
	};

} }