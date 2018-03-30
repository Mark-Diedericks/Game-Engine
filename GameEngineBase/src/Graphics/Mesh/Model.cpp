#include "ge.h"
#include "Model.h"

#include "System/Memory.h"
#include "Graphics/Shader/ShaderManager.h"
#include "System/VirtualFileSystem.h"
#include "Utils\LogUtil.h"

namespace gebase { namespace graphics {

	struct ModelFormat
	{
		char* header = "SPMF";

		byte nameLength;
		char* name;

		uint attributes;

		uint vertexBufferSize;
		byte* vertexData;

		uint indexBufferSize;
		byte* indexData;

		char* footer = "1234";
	};

	Model::Model(const String& path, MaterialInstance* materialInstance) : IRenderable()
	{
		Load(path);
		m_Mesh->setMaterial(materialInstance);
	}

	Model::~Model()
	{
		delete m_Mesh;
	}

	void Model::Render(Renderer3D& renderer)
	{
		m_Mesh->Render(renderer);
	}

	byte* ReadBytes(FILE* file, byte* buffer, uint size)
	{
		fread(buffer, 1, size, file);
		return buffer;
	}

	void Model::Load(const String& path)
	{
		String actualPath;
		VirtualFileSystem::Get()->ResolveActualPath(path, actualPath);

		ModelFormat format;
		FILE* f = fopen(actualPath.c_str(), "rb");

		if (!f)
		{
			utils::LogUtil::WriteLine("ERROR", "[Model] Load() - File is null, " + path + "    " + actualPath);
#ifdef GE_DEBUG
			__debugbreak();
#endif
		} 
		
		//ONE
		{
			byte header[4];
			ReadBytes(f, header, 4);
			if (memcmp(header, format.header, 4) != 0)
			{
				utils::LogUtil::WriteLine("ERROR", "[Model] Load() - Headers do not compare, " + (String)(char*)header + "    " + format.header);
#ifdef GE_DEBUG
				__debugbreak();
#endif
			}
		}

		//TWO
		{
			byte buffer[1];
			ReadBytes(f, buffer, 1);
			format.nameLength = *buffer;
		}

		//THREE
		{
			format.name = genew char[format.nameLength + 1];
			ReadBytes(f, (byte*)format.name, format.nameLength);
			format.name[format.nameLength] = '\0';
		}

		//FOUR
		{
			byte buffer[4];
			ReadBytes(f, buffer, 4);
			format.attributes = *(uint*)buffer;
		}

		//FIVE
		{
			byte buffer[4];
			ReadBytes(f, buffer, 4);
			format.vertexBufferSize = *(uint*)buffer;
		}

		//SIX
		{
			format.vertexData = genew byte[format.vertexBufferSize];
			ReadBytes(f, format.vertexData, format.vertexBufferSize);
		}

		//SEVEN
		{
			byte buffer[4];
			ReadBytes(f, buffer, 4);
			format.indexBufferSize = *(uint*)buffer;
		}

		//EIGHT
		{
			format.indexData = genew byte[format.indexBufferSize];
			ReadBytes(f, format.indexData, format.indexBufferSize);
		}

		//NINE
		{
			byte footer[4];
			ReadBytes(f, footer, 4);
			if (memcmp(footer, format.footer, 4) != 0)
			{
				utils::LogUtil::WriteLine("ERROR", "[Model] Load() - Footers do not compare, " + (String)(char*)footer + "    " + format.footer);
#ifdef GE_DEBUG
				__debugbreak();
#endif
			}
		}

		fclose(f);

		Shader* shader = ShaderManager::Get("AdvancedLighting");
		shader->Bind();

		VertexBuffer* vb = VertexBuffer::Create(BufferUsage::STATIC);
		vb->setData(format.vertexBufferSize, format.vertexData);

		BufferLayout layout;
		layout.Push<math::Vector3f>("POSITION");
		layout.Push<math::Vector3f>("NORMAL");
		layout.Push<math::Vector2f>("TEXCOORD");
		layout.Push<math::Vector3f>("BINORMAL");
		layout.Push<math::Vector3f>("TANGENT");
		vb->setLayout(layout, shader);

		VertexArray* va = VertexArray::Create();
		va->PushBuffer(vb);

		IndexBuffer* ib = IndexBuffer::Create((uint*)format.indexData, format.indexBufferSize / sizeof(uint));

		m_Mesh = genew Mesh(va, ib, nullptr);

#ifdef GE_DEBUG
		m_Mesh->setDebugData((Vertex*)format.vertexData, format.vertexBufferSize / sizeof(Vertex));
#endif
	}

	bool Model::EmployRenderAPIShader(RenderAPI api)
	{
		return m_Mesh->EmployRenderAPIShader(api);
	}

	bool Model::EmployRenderAPITexture2D(RenderAPI api)
	{
		return m_Mesh->EmployRenderAPITexture2D(api);
	}

	bool Model::EmployRenderAPITextureCube(RenderAPI api)
	{
		return m_Mesh->EmployRenderAPITextureCube(api);
	}

	bool Model::EmployRenderAPITextureDepth(RenderAPI api)
	{
		return m_Mesh->EmployRenderAPITextureDepth(api);
	}

	bool Model::EmployRenderAPIFramebuffer2D(RenderAPI api)
	{
		return m_Mesh->EmployRenderAPIFramebuffer2D(api);
	}

	bool Model::EmployRenderAPIFramebufferDepth(RenderAPI api)
	{
		return m_Mesh->EmployRenderAPIFramebufferDepth(api);
	}

	bool Model::EmployRenderAPIIndexBuffer(RenderAPI api)
	{
		return m_Mesh->EmployRenderAPIIndexBuffer(api);
	}

	bool Model::EmployRenderAPIVertexArray(RenderAPI api)
	{
		return m_Mesh->EmployRenderAPIVertexArray(api);
	}


} }