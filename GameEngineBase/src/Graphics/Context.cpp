#include "ge.h"
#include "Context.h"
#include "Renderer/Renderer.h"
#include "Backend/API/APIContext.h"
#include "IRenderableBase.h"
#include "Renderer/IRenderer.h"
#include "System/Memory.h"
#include "Application/Application.h"

#include "Buffer/IndexBuffer.h"
#include "Buffer/VertexBuffer.h"
#include "Buffer/VertexArray.h"

#include "Framebuffer/Framebuffer2D.h"
#include "Framebuffer/FramebufferDepth.h"

#include "Shader/Shader.h"

#include "Texture/Texture2D.h"
#include "Texture/TextureDepth.h"
#include "Texture/TextureCube.h"

#include "Debug/DebugRenderer.h"

namespace gebase { namespace graphics {

	RenderAPI Context::s_RenderAPI = RenderAPI::NONE;
	RenderAPI Context::s_PreviousRenderAPI = RenderAPI::NONE;
	RenderAPI Context::s_DefaultRenderAPI = RenderAPI::OPENGL;

	void* Context::s_DeviceContext = nullptr;
	WindowProperties Context::s_Properties;

	std::vector<IRenderer*> Context::s_RendererObjects;
	std::vector<IRenderableBase*> Context::s_RenderableObjects;

	void Context::Create(WindowProperties properties, void* deviceContext)
	{
		s_Properties = properties;
		s_DeviceContext = deviceContext;
		APIContext::Create(properties, deviceContext);
	}

	bool Context::EmployRenderAPI(RenderAPI api)
	{
		setRenderAPI(api);

		for (uint i = 0; i < s_RendererObjects.size(); i++)
			if (!s_RendererObjects[i]->PreEmployRenderAPI())
				return false;

		APIContext::Create(s_Properties, s_DeviceContext);

		if (!Renderer::EmployRenderAPI(api))
			return false;

		/*
		**  ------------------- TODO --------------------
		**  HAVE EACH RENDER API DEPENDANT SWITCH OVER AUTOMATICALLY
		**  THEN HAVE REFERENCING OBJECTS ATTEMPT TO FIND THE NEW IMPLEMENTATION
		**  THE SWAPOVER WOULD OCCUR HERE, WHEN THE NEW CONTEXT AND RENDERER ARE AVAILABLE
		*/ 

		const uint RendererObjectsSize = s_RendererObjects.size();
		const uint RenderableObjectsSize = s_RenderableObjects.size();
		const uint MAX_SIZE = RendererObjectsSize > RenderableObjectsSize ? RendererObjectsSize : RenderableObjectsSize;

		APIRenderer::DestroyPrevious();

		EmployRenderAPIShader(api, MAX_SIZE, RendererObjectsSize, RenderableObjectsSize);

		EmployRenderAPITexture2D(api, MAX_SIZE, RendererObjectsSize, RenderableObjectsSize);
		EmployRenderAPITextureCube(api, MAX_SIZE, RendererObjectsSize, RenderableObjectsSize);
		EmployRenderAPITextureDepth(api, MAX_SIZE, RendererObjectsSize, RenderableObjectsSize);

		EmployRenderAPIFramebuffer2D(api, MAX_SIZE, RendererObjectsSize, RenderableObjectsSize);
		EmployRenderAPIFramebufferDepth(api, MAX_SIZE, RendererObjectsSize, RenderableObjectsSize);

		EmployRenderAPIIndexBuffer(api, MAX_SIZE, RendererObjectsSize, RenderableObjectsSize);
		EmployRenderAPIVertexArray(api, MAX_SIZE, RendererObjectsSize, RenderableObjectsSize);

		//debug::DebugRenderer::EmployRenderAPI(api);

		return true;
	}

	bool Context::RevertRenderAPI()
	{
		return EmployRenderAPI(s_PreviousRenderAPI != RenderAPI::NONE ? s_PreviousRenderAPI : s_DefaultRenderAPI);
	}

	void Context::FlushRenderAPIChange(RenderAPI prevApi)
	{
		APIContext::DestroyPrevious();
	}

	void Context::Add(IRenderer* object)
	{
		s_RendererObjects.push_back(object);
	}

	void Context::Remove(IRenderer* object)
	{
		for (uint i = 0; i < s_RendererObjects.size(); i++)
			if (s_RendererObjects[i] == object)
				s_RendererObjects.erase(s_RendererObjects.begin() + i);
	}

	void Context::Add(IRenderableBase* object)
	{
		s_RenderableObjects.push_back(object);
	}

	void Context::Remove(IRenderableBase* object)
	{
		for (uint i = 0; i < s_RenderableObjects.size(); i++)
			if (s_RenderableObjects[i] == object)
				s_RenderableObjects.erase(s_RenderableObjects.begin() + i);
	}

	bool Context::EmployRenderAPIShader(RenderAPI api, const uint MAX_SIZE, const uint RendererObjectsSize, const uint RenderableObjectsSize)
	{
		Shader::PrepareRenderAPIChange(api);

		for (uint i = 0; i < MAX_SIZE; i++)
		{
			if (i < RendererObjectsSize)
			{
				if (s_RendererObjects[i])
					if (!s_RendererObjects[i]->EmployRenderAPIShader(api))
						return false;
			}

			if (i < RenderableObjectsSize)
			{
				if (s_RenderableObjects[i])
					if (!s_RenderableObjects[i]->EmployRenderAPIShader(api))
						return false;
			}
		}

		return true;
	}

	bool Context::EmployRenderAPITexture2D(RenderAPI api, const uint MAX_SIZE, const uint RendererObjectsSize, const uint RenderableObjectsSize)
	{
		Texture2D::PrepareRenderAPIChange(api);

		for (uint i = 0; i < MAX_SIZE; i++)
		{
			if (i < RendererObjectsSize)
			{
				if (s_RendererObjects[i])
					if (!s_RendererObjects[i]->EmployRenderAPITexture2D(api))
						return false;
			}

			if (i < RenderableObjectsSize)
			{
				if (s_RenderableObjects[i])
					if (!s_RenderableObjects[i]->EmployRenderAPITexture2D(api))
						return false;
			}
		}

		Texture2D::FlushRenderAPIChange(s_PreviousRenderAPI);

		return true;
	}

	bool Context::EmployRenderAPITextureCube(RenderAPI api, const uint MAX_SIZE, const uint RendererObjectsSize, const uint RenderableObjectsSize)
	{
		TextureCube::PrepareRenderAPIChange(api);

		for (uint i = 0; i < MAX_SIZE; i++)
		{
			if (i < RendererObjectsSize)
			{
				if (s_RendererObjects[i])
					if (!s_RendererObjects[i]->EmployRenderAPITextureCube(api))
						return false;
			}

			if (i < RenderableObjectsSize)
			{
				if (s_RenderableObjects[i])
					if (!s_RenderableObjects[i]->EmployRenderAPITextureCube(api))
						return false;
			}
		}

		TextureCube::FlushRenderAPIChange(s_PreviousRenderAPI);
		
		return true;
	}

	bool Context::EmployRenderAPITextureDepth(RenderAPI api, const uint MAX_SIZE, const uint RendererObjectsSize, const uint RenderableObjectsSize)
	{
		TextureDepth::PrepareRenderAPIChange(api);

		for (uint i = 0; i < MAX_SIZE; i++)
		{
			if (i < RendererObjectsSize)
			{
				if (s_RendererObjects[i])
					if (!s_RendererObjects[i]->EmployRenderAPITextureDepth(api))
						return false;
			}

			if (i < RenderableObjectsSize)
			{
				if (s_RenderableObjects[i])
					if (!s_RenderableObjects[i]->EmployRenderAPITextureDepth(api))
						return false;
			}
		}

		TextureDepth::FlushRenderAPIChange(s_PreviousRenderAPI);

		return true;
	}

	bool Context::EmployRenderAPIFramebuffer2D(RenderAPI api, const uint MAX_SIZE, const uint RendererObjectsSize, const uint RenderableObjectsSize)
	{
		Framebuffer2D::PrepareRenderAPIChange(api);

		for (uint i = 0; i < MAX_SIZE; i++)
		{
			if (i < RendererObjectsSize)
			{
				if (s_RendererObjects[i])
					if (!s_RendererObjects[i]->EmployRenderAPIFramebuffer2D(api))
						return false;
			}

			if (i < RenderableObjectsSize)
			{
				if (s_RenderableObjects[i])
					if (!s_RenderableObjects[i]->EmployRenderAPIFramebuffer2D(api))
						return false;
			}
		}

		Framebuffer2D::FlushRenderAPIChange(s_PreviousRenderAPI);

		return true;
	}

	bool Context::EmployRenderAPIFramebufferDepth(RenderAPI api, const uint MAX_SIZE, const uint RendererObjectsSize, const uint RenderableObjectsSize)
	{
		FramebufferDepth::PrepareRenderAPIChange(api);

		for (uint i = 0; i < MAX_SIZE; i++)
		{
			if (i < RendererObjectsSize)
			{
				if (s_RendererObjects[i])
					if (!s_RendererObjects[i]->EmployRenderAPIFramebufferDepth(api))
						return false;
			}

			if (i < RenderableObjectsSize)
			{
				if (s_RenderableObjects[i])
					if (!s_RenderableObjects[i]->EmployRenderAPIFramebufferDepth(api))
						return false;
			}
		}

		FramebufferDepth::FlushRenderAPIChange(s_PreviousRenderAPI);

		return true;
	}

	bool Context::EmployRenderAPIIndexBuffer(RenderAPI api, const uint MAX_SIZE, const uint RendererObjectsSize, const uint RenderableObjectsSize)
	{
		IndexBuffer::PrepareRenderAPIChange(api);

		for (uint i = 0; i < MAX_SIZE; i++)
		{
			if (i < RendererObjectsSize)
			{
				if (s_RendererObjects[i])
					if (!s_RendererObjects[i]->EmployRenderAPIIndexBuffer(api))
						return false;
			}

			if (i < RenderableObjectsSize)
			{
				if (s_RenderableObjects[i])
					if (!s_RenderableObjects[i]->EmployRenderAPIIndexBuffer(api))
						return false;
			}
		}

		IndexBuffer::FlushRenderAPIChange(s_PreviousRenderAPI);

		return true;
	}

	bool Context::EmployRenderAPIVertexArray(RenderAPI api, const uint MAX_SIZE, const uint RendererObjectsSize, const uint RenderableObjectsSize)
	{
		VertexBuffer::PrepareRenderAPIChange(api);
		VertexArray::PrepareRenderAPIChange(api); 

		for (uint i = 0; i < MAX_SIZE; i++)
		{
			if (i < RendererObjectsSize)
			{
				if (s_RendererObjects[i])
					if (!s_RendererObjects[i]->EmployRenderAPIVertexArray(api))
						return false;
			}

			if (i < RenderableObjectsSize)
			{
				if (s_RenderableObjects[i])
					if (!s_RenderableObjects[i]->EmployRenderAPIVertexArray(api))
						return false;
			}
		}

		Shader::FlushRenderAPIChange(s_PreviousRenderAPI);

		VertexBuffer::FlushRenderAPIChange(s_PreviousRenderAPI);
		VertexArray::FlushRenderAPIChange(s_PreviousRenderAPI);

		return true;
	}

} }