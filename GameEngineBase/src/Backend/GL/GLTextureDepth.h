#pragma once

#include "Backend/API/APITextureDepth.h"
#include "GLCommon.h"

namespace gebase { namespace graphics { namespace API {

	class GLTextureDepth : public APITextureDepth
	{
	private:
		String m_Name;
		uint m_Handle;
		uint m_Width;
		uint m_Height;
	protected:
		uint Init();
	public:
		GLTextureDepth(uint width, uint height);
		~GLTextureDepth();

		void Bind(uint slot = 0) const override;
		void Unbind(uint slot = 0) const override;

		void getPixelData(uint16* pixels) override;
		uint getSize() const override; 
		void setData(const uint16* data) override;

		inline uint getHandle() const { return m_Handle; }
		inline uint getWidth() const override { return m_Width; }
		inline uint getHeight() const override { return m_Height; }

		inline const String& getName() const override { return m_Name; }
		inline const String& getFilepath() const override { return m_Name; }
	};

} } }