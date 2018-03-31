#pragma once

#include "Common.h"
#include "Graphics/Buffer/VertexArray.h"
#include "Graphics/Buffer/IndexBuffer.h"
#include "Graphics/Material/Material.h"
#include "Graphics/Renderable/IRenderable.h"

namespace gebase { namespace graphics {

	class Renderer3D;

	struct GE_API Vertex
	{
		math::Vector3f position;
		math::Vector3f normal;
		math::Vector2f uv;
		math::Vector3f binormal;
		math::Vector3f tangent;
	};

	struct GE_API QuadVertex
	{
		math::Vector3f position;
		math::Vector2f uv;
	};

	class GE_API Mesh : public IRenderable
	{
	private:
		VertexArray* m_VertexArray;
		IndexBuffer* m_IndexBuffer;
		uint m_IndexBufferCount;
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

		bool EmployRenderAPIShader(RenderAPI api) override;
		bool EmployRenderAPITexture2D(RenderAPI api) override;
		bool EmployRenderAPITextureCube(RenderAPI api) override;
		bool EmployRenderAPITextureDepth(RenderAPI api) override;
		bool EmployRenderAPIFramebuffer2D(RenderAPI api) override;
		bool EmployRenderAPIFramebufferDepth(RenderAPI api) override;
		bool EmployRenderAPIIndexBuffer(RenderAPI api) override;
		bool EmployRenderAPIVertexArray(RenderAPI api) override;

		inline VertexArray* getVertexArray() const { return m_VertexArray; }
		inline IndexBuffer* getIndexBuffer() const { return m_IndexBuffer; }

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