#pragma once

#include "Texture.h"
#include "Backend/API/APITextureCube.h"

namespace gebase { namespace graphics {

	class GE_API TextureCube : public Texture
	{
	private:
		API::APITextureCube* m_Instance;
		String m_Name;
		byte** sides;
		uint m_BitsPerPixel;
		uint* m_Width;
		uint* m_Height;
		uint m_Mips;
		uint m_LoadType;
	public:
		bool EmployRenderAPI(RenderAPI api) override;

		inline void Bind(uint slot = 0) const override { m_Instance->Bind(slot); }
		inline void Unbind(uint slot = 0) const override { m_Instance->Unbind(slot); }

		inline const String& getName() const override { return m_Instance->getName(); }
		inline const String& getFilepath() const override { return m_Instance->getFilepath(); }

		inline API::APITexture* getInstance() override { return m_Instance; }

		static TextureCube* CreateFromFile(const String& filepath);
		static TextureCube* CreateFromFiles(const String* files);
		static TextureCube* CreateFromVerticalCross(const String* files, int32 mips);
	};

} } 