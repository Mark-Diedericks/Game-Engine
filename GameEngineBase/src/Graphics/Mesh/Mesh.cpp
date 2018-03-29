#include "ge.h"
#include "Mesh.h"

#include "Graphics/Renderer/Renderer3D.h"
#include "Debug/DebugRenderer.h"

namespace gebase { namespace graphics {

	Mesh::Mesh(VertexArray* vertexArray, IndexBuffer* indexBuffer, MaterialInstance* materialInstance) : IRenderable(), m_VertexArray(vertexArray), m_IndexBuffer(indexBuffer), m_MaterialInstance(materialInstance)
	{
#ifdef GE_DEBUG
		m_DebugVertexData = nullptr;
		m_DebugVertexDataCount = 0;
		m_DebugDraw = false;
#endif
	}

	Mesh::Mesh(const Mesh* mesh) : IRenderable(), m_VertexArray(mesh->m_VertexArray), m_IndexBuffer(mesh->m_IndexBuffer), m_MaterialInstance(mesh->m_MaterialInstance)
	{
#ifdef GE_DEBUG
		m_DebugVertexData = mesh->m_DebugVertexData;
		m_DebugVertexDataCount = mesh->m_DebugVertexDataCount;
		m_DebugDraw = mesh->m_DebugDraw;
#endif
	}

	Mesh::~Mesh()
	{
		delete m_VertexArray;
		delete m_IndexBuffer;
		delete m_MaterialInstance;

#ifdef GE_DEBUG
		delete[] m_DebugVertexData;
#endif
	}

	void Mesh::Render(Renderer3D& renderer)
	{
		m_MaterialInstance->Bind();

		m_VertexArray->Bind();
		m_IndexBuffer->Bind();

		m_VertexArray->Draw(m_IndexBuffer->getCount());

		m_IndexBuffer->Unbind();
		m_VertexArray->Unbind();

		m_MaterialInstance->Unbind();
	}

#ifdef GE_DEBUG
	void Mesh::DebugRender(const math::Matrix4f& transform)
	{
		if (!m_DebugDraw)
			return;

		debug::DebugRenderer::DrawMesh(this, debug::DebugRenderMeshFlags::NONE, transform);
	}
#endif

	bool Mesh::EmployRenderAPIShader(RenderAPI api)
	{
		if (m_MaterialInstance)
			if (!m_MaterialInstance->EmployRenderAPIShader(api))
				return false;

		return true;
	}

	bool Mesh::EmployRenderAPITexture2D(RenderAPI api)
	{
		if (m_MaterialInstance)
			if (!m_MaterialInstance->EmployRenderAPITexture2D(api))
				return false;

		return true;
	}

	bool Mesh::EmployRenderAPITextureCube(RenderAPI api)
	{
		if (m_MaterialInstance)
			if (!m_MaterialInstance->EmployRenderAPITextureCube(api))
				return false;

		return true;
	}

	bool Mesh::EmployRenderAPITextureDepth(RenderAPI api)
	{
		if (m_MaterialInstance)
			if (!m_MaterialInstance->EmployRenderAPITextureDepth(api))
				return false;

		return true;
	}

	bool Mesh::EmployRenderAPIFramebuffer2D(RenderAPI api)
	{
		if (m_MaterialInstance)
			if (!m_MaterialInstance->EmployRenderAPIFramebuffer2D(api))
				return false;

		return true;
	}

	bool Mesh::EmployRenderAPIFramebufferDepth(RenderAPI api) 
	{
		if (m_MaterialInstance)
			if (!m_MaterialInstance->EmployRenderAPIFramebufferDepth(api))
				return false;

		return true;
	}

	bool Mesh::EmployRenderAPIIndexBuffer(RenderAPI api)
	{
		if (m_IndexBuffer)
			m_IndexBuffer = IndexBuffer::ConvertRenderAPI(api, m_IndexBuffer);

		return true;
	}

	bool Mesh::EmployRenderAPIVertexBuffer(RenderAPI api)
	{
		if (m_VertexArray)
			for (uint i = 0; i < m_VertexArray->getBuffers().size(); i++)
				VertexBuffer::ConvertRenderAPI(api, m_VertexArray->getBuffer(i));

		return true;
	}

	bool Mesh::EmployRenderAPIVertexArray(RenderAPI api) 
	{
		if (m_VertexArray)
			m_VertexArray = VertexArray::ConvertRenderAPI(api, m_VertexArray);

		return true;
	}


} }