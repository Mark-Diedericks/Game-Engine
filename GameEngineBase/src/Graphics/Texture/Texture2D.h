#pragma once

#include "Texture.h"

namespace gebase { namespace graphics {

	class GE_API Texture2D : public Texture
	{
	protected:
		TextureParameters m_Parameters;

		Texture2D(uint loadType) : Texture(TextureType::TEX2D, loadType) {}
	public:
		static Texture2D* CreateFromFile(const String& filepath, TextureParameters parameters = TextureParameters(), TextureLoadOptions loadOptions = TextureLoadOptions());
		static Texture2D* CreateFromFile(const String& filepath, TextureLoadOptions loadOptions);
		static Texture2D* CreateFromFile(const String& name, const String& filepath, TextureParameters parameters = TextureParameters(), TextureLoadOptions loadOptions = TextureLoadOptions());

		static Texture2D* Create(uint width, uint height, TextureParameters parameters = TextureParameters());
		static Texture2D* CreateFromFile(const String& name, const byte* pixels, uint width, uint height, uint bits, TextureParameters parameters = TextureParameters());

		static Texture2D* ConvertRenderAPI(RenderAPI api, Texture2D* original);
		
		virtual void getPixelData(byte* pixels) = 0;
		virtual uint getSize() const = 0;

		virtual void setData(const byte* pixels, const bool del = true) = 0;
		virtual void setData(uint color) = 0;

		virtual uint getWidth() const = 0;
		virtual uint getHeight() const = 0;
	private:
		static std::vector<Texture2D*> s_Current;
		static std::map<Texture2D*, Texture2D*> s_APIChangeMap;
	public:
		static inline void AddRenderAPIChange(Texture2D* old, Texture2D* current) { s_APIChangeMap.insert_or_assign(old, current); }
		static inline bool HasRenderAPIChange(Texture2D* old) { return s_APIChangeMap.find(old) != s_APIChangeMap.end(); }
		static inline Texture2D* GetRenderAPIChange(Texture2D* old) { return s_APIChangeMap.at(old); }
		static void PrepareRenderAPIChange(RenderAPI newApi);
		static void FlushRenderAPIChange(RenderAPI prevApi);
	};

} }