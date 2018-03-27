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
		RenderAPI prevAPI = getRenderAPI();
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

		IndexBuffer::PrepareRenderAPIChange(api);
		VertexBuffer::PrepareRenderAPIChange(api);
		VertexArray::PrepareRenderAPIChange(api);
		Framebuffer2D::PrepareRenderAPIChange(api);
		FramebufferDepth::PrepareRenderAPIChange(api);
		Shader::PrepareRenderAPIChange(api);
		Texture2D::PrepareRenderAPIChange(api);
		TextureCube::PrepareRenderAPIChange(api);
		TextureDepth::PrepareRenderAPIChange(api);

		for (uint i = 0; i < s_RendererObjects.size(); i++)
			if(s_RendererObjects[i])
			if (!s_RendererObjects[i]->EmployRenderAPI(api))
				return false;

		for (uint i = 0; i < s_RenderableObjects.size(); i++)
			if(s_RenderableObjects[i])
				if (!s_RenderableObjects[i]->EmployRenderAPI(api))
					return false;

		IndexBuffer::FlushRenderAPIChange(prevAPI);
		VertexBuffer::FlushRenderAPIChange(prevAPI);
		VertexArray::FlushRenderAPIChange(prevAPI);
		Framebuffer2D::FlushRenderAPIChange(prevAPI);
		FramebufferDepth::FlushRenderAPIChange(prevAPI);
		Shader::FlushRenderAPIChange(prevAPI);
		Texture2D::FlushRenderAPIChange(prevAPI);
		TextureCube::FlushRenderAPIChange(prevAPI);
		TextureDepth::FlushRenderAPIChange(prevAPI);

		return true;
	}

	bool Context::RevertRenderAPI()
	{
		return EmployRenderAPI(s_PreviousRenderAPI != RenderAPI::NONE ? s_PreviousRenderAPI : s_DefaultRenderAPI);
	}

	void Context::FlushRenderAPIChange(RenderAPI prevApi)
	{
		APIRenderer::DestroyPrevious();
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

} }