#pragma once

#include "Graphics/Context/Context.h"
#include "GLCommon.h"

namespace gebase { namespace graphics {

	class GLContext : public Context
	{
	private:
		void setVSync(bool enabled);
	protected:
		void DestroyInternal() override;
	public:
		GLContext(WindowProperties properties, void* deviceContext);
		~GLContext();
		
		void Present();

		inline static GLContext* Get() { return (GLContext*)s_Context; }
	};

} }