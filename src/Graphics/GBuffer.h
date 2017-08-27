#pragma once

#include "Framebuffer/Framebuffer.h"

namespace gebase { namespace graphics {

	class GE_API GBuffer
	{
	public:
		enum class TextureType
		{
			Position = 0,
			Diffuse,
			Normal,
			TextureCoord
		};
	private:
		uint m_Framebuffer;
		uint m_Textures[4];
		uint m_DepthTexture;
		uint m_Width;
		uint m_Height;

		void Init();
	public:
		GBuffer(uint width, uint height);
		~GBuffer();

		void Bind(int32 mode = 0);
		void setReadBuffer(TextureType type);

		inline uint getTexture(uint index) const { return m_Textures[index]; }

		inline uint getWidth() const { return m_Width; }
		inline uint getHeight() const { return m_Height; }
	};

} }