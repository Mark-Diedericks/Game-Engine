#pragma once

#include "Backend/API/APIFramebufferDepth.h"
#include "Graphics/Texture/TextureDepth.h"
#include "GLTextureDepth.h"
#include "GLCommon.h"

namespace gebase { namespace graphics { namespace API {

	class GLFramebufferDepth : public APIFramebufferDepth
	{
	private:
		uint m_FramebufferHandle;

		uint m_Width;
		uint m_Height;

		TextureDepth* m_Texture;

		void Init();
	public:
		GLFramebufferDepth(uint width, uint height);
		~GLFramebufferDepth();

		void Bind() const override;
		void Unbind() const override;
		void Clear() override;
		
		uint16* getPixelData() override;
		void setData(const void* data) override;

		inline uint getWidth() const override { return m_Width; }
		inline uint getHeight() const override { return m_Height; }
		inline Texture* getTexture() const override { return m_Texture; }
	};

} } }