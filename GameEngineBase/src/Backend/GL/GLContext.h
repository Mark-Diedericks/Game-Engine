#pragma once

#include "Graphics/Context/Context.h"
#include "GLCommon.h"

namespace gebase { namespace graphics {

	class GLContext : public Context
	{
	private:
		void setVSync(bool enabled);
		static GLContext* s_Instance;
	protected:
		void DestroyInternal() override;
		void PresentInternal() override;
	public:
		GLContext(WindowProperties properties, void* deviceContext);
		~GLContext();

		inline static GLContext* Get() { return s_Instance; }
	};

} }