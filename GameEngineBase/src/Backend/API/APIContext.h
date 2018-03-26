#pragma once

#include "Common.h"
#include "Application/Window.h"
#include "System/Memory.h"

namespace gebase { namespace graphics {

	class APIContext
	{
	protected:
		static APIContext* s_Context;
		static APIContext* s_PreviousContext;
		virtual void DestroyInternal() = 0;
	public:
		static void Create(WindowProperties properties, void* deviceContext);
		inline static void Destroy() { s_Context->DestroyInternal(); }
		inline static void DestroyPrevious() 
		{ 
			if (s_PreviousContext != nullptr) 
			{
				s_PreviousContext->DestroyInternal();
				gedel s_PreviousContext;
			}

		}
	};

} }