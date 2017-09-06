#pragma once

#include "Backend/API/APIContext.h"
#include "GLCommon.h"

namespace gebase { namespace graphics { namespace API {

	class GLContext : public APIContext
	{
	private:
		void setVSync(bool enabled);
	public:
		GLContext(WindowProperties properties, void* deviceContext);
		~GLContext();
		void Present();
		inline static GLContext* Get() { return (GLContext*)s_Context; }
	};

} } }