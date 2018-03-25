#pragma once

#include "Graphics/Framebuffer/Framebuffer2D.h"
#include "Graphics/Texture/Texture2D.h"
#include "GLTexture2D.h"
#include "Math/Maths.h"
#include "GLCommon.h"

namespace gebase { namespace graphics {

	class GLFramebuffer2D : public Framebuffer2D
	{
	private:
		uint m_FramebufferHandle;
		uint m_DepthbufferHandle;

		uint m_Width;
		uint m_Height;

		Texture2D* m_Texture;

		void Init();
	public:
		GLFramebuffer2D(uint width, uint height);
		~GLFramebuffer2D();

		void Bind() const override;
		void Unbind() const override;
		void Clear() override;

		void getPixelData(byte* data) override;
		void setData(const byte* data) override;

		inline uint getWidth() const override { return m_Width; }
		inline uint getHeight() const override { return m_Height; }

		inline void setClearColor(const math::Vector4f& color) override { m_ClearColor = color; }
		inline const math::Vector4f& getColor() const { return m_ClearColor; }

		inline Texture* getTexture() const override { return m_Texture; }
	};

} }