#pragma once

#include "Backend/API/APIFramebuffer2D.h"
#include "Framebuffer.h"

namespace gebase { namespace graphics {

	class GE_API Framebuffer2D : public Framebuffer
	{
	private:
		API::APIFramebuffer2D* m_Instance;
	public:
		inline void setClearColor(const math::Vector4f& color) { m_Instance->setClearColor(color); }
		 
		inline void Bind() const override { m_Instance->Bind(); }
		inline void Unbind() const override { m_Instance->Unbind(); }
		inline void Clear() override { m_Instance->Clear(); }

		inline Texture* getTexture() const override { return m_Instance->getTexture(); }
		inline uint getWidth() const override { return m_Instance->getWidth(); }
		inline uint getHeight() const override { return m_Instance->getHeight(); }

		inline API::APIFramebuffer* getInstance() override { return m_Instance; }

		static Framebuffer2D* Create(uint width, uint height);
	};

} }