#pragma once

#include "Graphics/Framebuffer/FramebufferDepth.h"

namespace gebase { namespace graphics { 

	class DX11FramebufferDepth : public FramebufferDepth
	{
	private:
		uint m_Width;
		uint m_Height;

		void Init();
	public:
		DX11FramebufferDepth(uint width, uint height);
		~DX11FramebufferDepth();

		void Bind() const override;
		void Unbind() const override;
		void Clear() override;

		void getPixelData(uint16*) override;
		void setData(const uint16* data) override;

		inline uint getWidth() const override { return m_Width; }
		inline uint getHeight() const override { return m_Height; }
		inline Texture* getTexture() const override { return nullptr; }
	};

} }