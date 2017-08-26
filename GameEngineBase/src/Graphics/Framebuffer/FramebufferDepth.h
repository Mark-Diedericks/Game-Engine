#pragma once

#include "Backend/API/APIFramebufferDepth.h"
#include "Framebuffer.h"

namespace gebase { namespace graphics {

	class FramebufferDepth : public Framebuffer
	{
	private:
		API::APIFramebufferDepth* m_Instance;
	public:
		static FramebufferDepth* Create(uint width, uint height);

		inline void Bind() const override { m_Instance->Bind(); }
		inline void Unbind() const override { m_Instance->Unbind(); }
		inline void Clear() override { m_Instance->Clear(); }

		inline Texture* getTexture() const override { return m_Instance->getTexture(); }
		inline uint getWidth() const override { return m_Instance->getWidth(); }
		inline uint getHeight() const override { return m_Instance->getHeight(); }

		inline API::APIFramebuffer* getInstance() override { return m_Instance; }
	};

} }