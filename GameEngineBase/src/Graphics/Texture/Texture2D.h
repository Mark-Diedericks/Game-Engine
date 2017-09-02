#pragma once

#include "Texture.h"
#include "Backend/API/APITexture2D.h"

namespace gebase { namespace graphics {

	class GE_API Texture2D : public Texture
	{
	private:
		API::APITexture2D* m_Instance;

		String m_Name;
		uint m_BitsPerPixel;
		uint m_Width;
		uint m_Height;

		API::TextureParameters m_Parameters;

		Texture2D() : Texture() {}
	public:
		void setData(const byte* pixels);
		void setData(uint color);

		bool EmployRenderAPI(RenderAPI api) override;

		inline byte* getPixelData() { return m_Instance->getPixelData(); }
		inline void setData(const void* data) { m_Instance->setData(data); }

		inline uint getWidth() const { return m_Instance->getWidth(); }
		inline uint getHeight() const { return m_Instance->getHeight(); }

		inline void Bind(uint slot = 0) const override { m_Instance->Bind(slot); }
		inline void Unbind(uint slot = 0) const override { m_Instance->Unbind(slot); }

		inline const String& getName() const override { return m_Instance->getName(); }
		inline const String& getFilepath() const override { return m_Instance->getFilepath(); }

		inline API::APITexture* getInstance() override { return m_Instance; }

		static Texture2D* Create(uint width, uint height, API::TextureParameters parameters = API::TextureParameters(), API::TextureLoadOptions loadOptions = API::TextureLoadOptions());

		static Texture2D* CreateFromFile(const String& filepath, API::TextureParameters parameters = API::TextureParameters(), API::TextureLoadOptions loadOptions = API::TextureLoadOptions());
		static Texture2D* CreateFromFile(const String& filepath, API::TextureLoadOptions loadOptions);
		static Texture2D* CreateFromFile(const String& name, const String& filepath, API::TextureParameters parameters = API::TextureParameters(), API::TextureLoadOptions loadOptions = API::TextureLoadOptions());
		static Texture2D* CreateFromFile(const String& name, const String& filepath, API::TextureLoadOptions loadOptions);
	};

} }