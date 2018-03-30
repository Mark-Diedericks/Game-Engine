#pragma once

#include "Common.h"
#include "Application/Window.h"
#include "System/Memory.h"

namespace gebase { namespace graphics { 

	class IRenderAPIDependant;
	class IRenderer;
	class IRenderableBase;

	enum class GE_API RenderAPI
	{
		NONE,
		OPENGL,
		VULKAN,
		D3D11,
		D3D12
	};

	class GE_API Context
	{
	private:
		static RenderAPI s_RenderAPI;
		static RenderAPI s_PreviousRenderAPI;
		static RenderAPI s_DefaultRenderAPI;

		static void* s_DeviceContext;
		static WindowProperties s_Properties;

		static std::vector<IRenderer*> s_RendererObjects;
		static std::vector<IRenderableBase*> s_RenderableObjects;

		static bool EmployRenderAPIShader(RenderAPI api, const uint MAX_SIZE, const uint RendererObjectsSize, const uint RenderableObjectsSize);
		static bool EmployRenderAPITexture2D(RenderAPI api, const uint MAX_SIZE, const uint RendererObjectsSize, const uint RenderableObjectsSize);
		static bool EmployRenderAPITextureCube(RenderAPI api, const uint MAX_SIZE, const uint RendererObjectsSize, const uint RenderableObjectsSize);
		static bool EmployRenderAPITextureDepth(RenderAPI api, const uint MAX_SIZE, const uint RendererObjectsSize, const uint RenderableObjectsSize);
		static bool EmployRenderAPIFramebuffer2D(RenderAPI api, const uint MAX_SIZE, const uint RendererObjectsSize, const uint RenderableObjectsSize);
		static bool EmployRenderAPIFramebufferDepth(RenderAPI api, const uint MAX_SIZE, const uint RendererObjectsSize, const uint RenderableObjectsSize);
		static bool EmployRenderAPIIndexBuffer(RenderAPI api, const uint MAX_SIZE, const uint RendererObjectsSize, const uint RenderableObjectsSize);
		static bool EmployRenderAPIVertexArray(RenderAPI api, const uint MAX_SIZE, const uint RendererObjectsSize, const uint RenderableObjectsSize);
	protected:
		static Context* s_Context;
		static Context* s_PreviousContext;

		virtual void DestroyInternal() = 0;
		inline static void DestroyPrevious()
		{
			if (s_PreviousContext != nullptr)
			{
				s_PreviousContext->DestroyInternal();
				gedel s_PreviousContext;
			}

		}
	public:
		static void Create(WindowProperties properties, void* deviceContext);
		inline static void Destroy() { s_Context->DestroyInternal(); }

		static RenderAPI getRenderAPI() { return s_RenderAPI; }
		static void setRenderAPI(RenderAPI api) { s_PreviousRenderAPI = s_RenderAPI;  s_RenderAPI = api; }

		static bool EmployRenderAPI(RenderAPI api);
		static bool RevertRenderAPI();
		static void FlushRenderAPIChange(RenderAPI prevApi);

		static void Add(IRenderer* object);
		static void Remove(IRenderer* object);

		static void Add(IRenderableBase* object);
		static void Remove(IRenderableBase* object);
	};

} }