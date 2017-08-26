#include "Pch.h"
#include "DebugRenderer.h"
#include "Graphics/Shader/ShaderFactory.h"

namespace gebase { namespace debug {

#ifdef GE_DEBUG

#define RENDERER_VERTEX_SIZE	sizeof(LineVertex)
#define RENDERER_MAX_LINES		65536
#define RENDERER_BUFFER_SIZE	RENDERER_VERTEX_SIZE * RENDERER_MAX_LINES
#define RENDERER_INDICES_SIZE	RENDERER_MAX_LINES * 2

	using namespace graphics;
	using namespace math;

	struct LineVertex
	{
		Vector3f position;
		uint color;
	};

	static Shader* s_Shader;
	static Camera* s_Camera;
	static VertexArray* s_VertexArray;
	static IndexBuffer* s_IndexBuffer;
	static LineVertex* s_LineVertexPointer;
	static uint s_IndexCount;

	void DebugRenderer::Init()
	{
		s_Camera = nullptr;
		s_Shader = ShaderFactory::DebugShader();

		VertexBuffer* vb = VertexBuffer::Create(API::BufferUsage::DYNAMIC);
		vb->Bind();
		vb->Resize(RENDERER_BUFFER_SIZE);

		API::APIBufferLayout layout;
		layout.Push<Vector3f>("position");
		layout.Push<byte>("color", 4, true);
		vb->setLayout(layout);

		s_VertexArray = VertexArray::Create();
		s_VertexArray->PushBuffer(vb);

		uint* indices = new uint[RENDERER_INDICES_SIZE];

		for (int32 i = 0; i < RENDERER_INDICES_SIZE; i++)
			indices[i] = i;

		s_IndexBuffer = IndexBuffer::Create(indices, RENDERER_INDICES_SIZE);
		
		vb->Unbind();
		s_VertexArray->Unbind();
		s_IndexCount = 0;
	}

	void DebugRenderer::Shutdown()
	{
		delete s_Shader;
		delete s_VertexArray;
		delete s_IndexBuffer;
	}

	static void Begin()
	{
		s_VertexArray->getBuffer()->Bind();
		s_LineVertexPointer = s_VertexArray->getBuffer()->getPointer<LineVertex>();
	}

	static void End()
	{
		s_VertexArray->getBuffer()->ReleasePointer();
		s_VertexArray->getBuffer()->Unbind();
	}

	void DebugRenderer::setCamera(Camera* camera)
	{
		Present();
		s_Camera = camera;
	}

	void DebugRenderer::Present()
	{
		if (!s_Camera)
			return;
	}

	void DebugRenderer::DrawLineInternal(const Vector3f& start, const Vector3f& end, uint color)
	{
		s_LineVertexPointer->position = start;
		s_LineVertexPointer->color = color;
		s_LineVertexPointer++;

		s_LineVertexPointer->position = end;
		s_LineVertexPointer->color = color;
		s_LineVertexPointer++;

		s_IndexCount += 2;

		if (s_IndexCount >= RENDERER_INDICES_SIZE)
		{
			End();
			Present();
			Begin();
		}
	}

	void DebugRenderer::DrawLine(const Vector3f& start, const Vector3f& end, uint color)
	{
		Begin();
		DrawLineInternal(start, end, color);
		End();
	}

	void DebugRenderer::DrawMesh(const Mesh* mesh, DebugRenderMeshFlags flags, const Matrix4f& transform)
	{
		Vertex* vertices = nullptr;
		uint count = mesh->getDebuData(vertices);
		float s = 2.0f;

		Begin();

		for (uint i = 0; i < count; i++)
		{
			Vertex& v = vertices[i];
			Vector3f pos = transform * v.position;

			DrawLineInternal(Vector3f(pos.x, pos.y, pos.z), Vector3f(pos.x + v.normal.x * s, pos.y + v.normal.y * s, pos.z * v.normal.z * s), 0xff0000ff);
			DrawLineInternal(Vector3f(pos.x, pos.y, pos.z), Vector3f(pos.x + v.binormal.x * s, pos.y + v.binormal.y * s, pos.z + v.binormal.z * s), 0xff00ff00);
			DrawLineInternal(Vector3f(pos.x, pos.y, pos.z), Vector3f(pos.x + v.tangent.x * s, pos.y + v.tangent.y * s, pos.z + v.tangent.z * s), 0xffff0000);
		}

		End();
	}

#endif

} }

