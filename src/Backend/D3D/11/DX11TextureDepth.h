#pragma once

#include "Backend/API/APITextureDepth.h"

namespace gebase { namespace graphics { namespace API {

	class DX11TextureDepth : public APITextureDepth
	{
	private:
		String m_Name;
		uint m_Width;
		uint m_Height;
	protected:
		void Init();
	public:
		DX11TextureDepth(uint width, uint height);
		~DX11TextureDepth();

		void Bind(uint slot = 0) const override;
		void Unbind(uint slot = 0) const override;

		inline const String& getName() const override { return m_Name; }
		inline const String& getFilepath() const override { return m_Name; }

		inline uint getWidth() const override { return m_Width; }
		inline uint getHeight() const override { return m_Height; }
	};

} } }