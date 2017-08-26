#pragma once

#include "Backend/API/APIFramebuffer2D.h"

namespace gebase { namespace graphics { namespace API {

	class DX11Framebuffer2D : public APIFramebuffer2D
	{
	private:
		uint m_Width;
		uint m_Height;
		
		void Init();
	public:
		DX11Framebuffer2D(uint width, uint height);
		~DX11Framebuffer2D();

		void Bind() const override;
		void Unbind() const override;
		void Clear() override;

		inline uint getWidth() const override { return m_Width; }
		inline uint getHeight() const override { return m_Height; }
		inline Texture* getTexture() const override { return nullptr; }

		inline void setClearColor(const math::Vector4f& color) override { m_ClearColor = color; }
		inline const math::Vector4f& getColor() const { return m_ClearColor; }
	};

} } }