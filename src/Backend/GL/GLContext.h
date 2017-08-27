#pragma once

#include "Backend/API/APIContext.h"
#include "GLCommon.h"

namespace gebase { namespace graphics { namespace API {

	class GLContext : public APIContext
	{
	public:
		GLContext(WindowProperties properties, void* deviceContext);
		void Present();
		inline static GLContext* Get() { return (GLContext*)s_Context; }
	};

} } }