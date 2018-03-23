#include "ge.h"
#include "MeshFactory.h"

namespace gebase { namespace graphics { namespace MeshFactory {

	Mesh* CreateQuad(float x, float y, float width, float height, MaterialInstance* material)
	{
		using namespace math;

		struct QuadVertex
		{
			math::Vector3f position;
			math::Vector2f uv;
		};

		QuadVertex vertices[4];

		vertices[0].position = Vector3f(x, y, 0);
		vertices[0].uv = Vector2f(0, 1);

		vertices[1].position = Vector3f(x + width, y, 0);
		vertices[1].uv = Vector2f(0, 0);

		vertices[2].position = Vector3f(x + width, y + height, 0);
		vertices[2].uv = Vector2f(1, 0);

		vertices[3].position = Vector3f(x, y + height, 0);
		vertices[3].uv = Vector2f(1, 1);

		VertexBuffer* vb = VertexBuffer::Create(API::BufferUsage::STATIC);
		vb->setData(sizeof(QuadVertex) * 4, vertices);

		API::APIBufferLayout layout;
		layout.Push<Vector3f>("POSITION");
		layout.Push<Vector2f>("TEXCOORD");
		vb->setLayout(layout);

		VertexArray* va = VertexArray::Create();
		va->PushBuffer(vb);

		uint* indices = new uint[6]{ 0, 1, 2, 2, 3, 0 };
		IndexBuffer* ib = IndexBuffer::Create(indices, 6);

		return new Mesh(va, ib, material);
	}

	Mesh* CreateQuad(const math::Vector2f& position, const math::Vector2f& size, MaterialInstance* material)
	{
		return CreateQuad(position.x, position.y, size.x, size.y, material);
	}

	Mesh* CreateCube(float size, MaterialInstance* material)
	{
		using namespace math;
		
		Vertex vertices[8];
		memset(vertices, 0, sizeof(Vertex) * 8);

		vertices[0].position = Vector3f(-size / 2.0f, -size / 2.0f, size / 2.0f);
		vertices[1].position = Vector3f(size / 2.0f, -size / 2.0f, size / 2.0f);
		vertices[2].position = Vector3f(size / 2.0f, size / 2.0f, size / 2.0f);
		vertices[3].position = Vector3f(-size / 2.0f, size / 2.0f, size / 2.0f);
		vertices[4].position = Vector3f(-size / 2.0f, -size / 2.0f, -size / 2.0f);
		vertices[5].position = Vector3f(size / 2.0f, -size / 2.0f, -size / 2.0f);
		vertices[6].position = Vector3f(size / 2.0f, size / 2.0f, -size / 2.0f);
		vertices[7].position = Vector3f(-size / 2.0f, size / 2.0f, -size / 2.0f);

		vertices[0].normal = Vector3f(-1.0f, -1.0f, 1.0f);
		vertices[1].normal = Vector3f(1.0f, -1.0f, 1.0f);
		vertices[2].normal = Vector3f(1.0f, 1.0f, 1.0f);
		vertices[3].normal = Vector3f(-1.0f, 1.0f, 1.0f);
		vertices[4].normal = Vector3f(-1.0f, -1.0f, -1.0f);
		vertices[5].normal = Vector3f(1.0f, -1.0f, -1.0f);
		vertices[6].normal = Vector3f(1.0f, 1.0f, -1.0f);
		vertices[7].normal = Vector3f(-1.0f, 1.0f, -1.0f);

		VertexBuffer* vb = VertexBuffer::Create(API::BufferUsage::STATIC);
		vb->setData(sizeof(Vertex) * 8, vertices);

		API::APIBufferLayout layout;
		layout.Push<Vector3f>("position");
		layout.Push<Vector3f>("normal");
		layout.Push<Vector2f>("uv");
		vb->setLayout(layout);

		VertexArray* va = VertexArray::Create();
		va->PushBuffer(vb);

		uint* indices = new uint[36]
		{
			0, 1, 2, 2, 3, 0,
			3, 2, 6, 6, 7, 3,
			7, 6, 5, 5, 4, 7,
			4, 0, 3, 3, 7, 4,
			0, 1, 5, 5, 4, 0,
			1, 5, 6, 6, 2, 1
		};
		IndexBuffer* ib = IndexBuffer::Create(indices, 36);

		return new Mesh(va, ib, material);
	}

	Mesh* CreatePlane(float width, float height, const math::Vector3f& normal, MaterialInstance* material)
	{
		using namespace math;

		Vector3f vec = normal * 90.0f;
		Matrix4f rotation = Matrix4f::Rotate(vec.z, Vector3f(1, 0, 0)) * Matrix4f::Rotate(vec.y, Vector3f(0, 1, 0)) * Matrix4f::Rotate(vec.x, Vector3f(0, 0, 1));

		Vertex vertices[4];
		memset(vertices, 0, 4 * sizeof(Vertex));

		vertices[0].position = rotation * Vector3f(-width / 2.0f, 0.0f, -height / 2.0f);
		vertices[0].normal = normal;
		vertices[0].uv = Vector2f(0.0f, 0.0f);
		vertices[0].binormal = Matrix4f::Rotate(90.0f, Vector3f(0, 1, 0)) * normal;
		vertices[0].tangent = Matrix4f::Rotate(90.0f, Vector3f(0, 0, 1)) * normal;
		
		vertices[1].position = rotation * Vector3f(-width / 2.0f, 0.0f, height / 2.0f);
		vertices[1].normal = normal;
		vertices[1].uv = Vector2f(0.0f, 1.0f);
		vertices[1].binormal = Matrix4f::Rotate(90.0f, Vector3f(0, 1, 0)) * normal;
		vertices[1].tangent = Matrix4f::Rotate(90.0f, Vector3f(0, 0, 1)) * normal;
		
		vertices[2].position = rotation * Vector3f(width / 2.0f, 0.0f, height / 2.0f);
		vertices[2].normal = normal;
		vertices[2].uv = Vector2f(1.0f, 1.0f);
		vertices[2].binormal = Matrix4f::Rotate(90.0f, Vector3f(0, 1, 0)) * normal;
		vertices[2].tangent = Matrix4f::Rotate(90.0f, Vector3f(0, 0, 1)) * normal;
		
		vertices[3].position = rotation * Vector3f(width / 2.0f, 0.0f, -height / 2.0f);
		vertices[3].normal = normal;
		vertices[3].uv = Vector2f(1.0f, 0.0f);
		vertices[3].binormal = Matrix4f::Rotate(90.0f, Vector3f(0, 1, 0)) * normal;
		vertices[3].tangent = Matrix4f::Rotate(90.0f, Vector3f(0, 0, 1)) * normal;

		material->Bind();

		VertexBuffer* vb = VertexBuffer::Create(API::BufferUsage::STATIC);
		vb->setData(sizeof(Vertex) * 8, vertices);

		API::APIBufferLayout layout;
		layout.Push<math::Vector3f>("POSITION");
		layout.Push<math::Vector3f>("NORMAL");
		layout.Push<math::Vector2f>("TEXCOORD");
		layout.Push<math::Vector3f>("BINORMAL");
		layout.Push<math::Vector3f>("TANGENT");
		vb->setLayout(layout);

		VertexArray* va = VertexArray::Create();
		va->PushBuffer(vb);

		uint* indices = new uint[6]{ 0, 1, 2, 2, 3, 0 };
		IndexBuffer* ib = IndexBuffer::Create(indices, 6);

		return new Mesh(va, ib, material);
	}

} } }