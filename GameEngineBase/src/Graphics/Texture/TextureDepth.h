#pragma once

#include "Texture.h"

namespace gebase { namespace graphics {

	class GE_API TextureDepth : public Texture
	{
	protected:
		TextureDepth(uint loadType) : Texture(TextureType::TEXDEPTH, loadType) {}
	public:
		static TextureDepth* Create(uint width, uint height);

		static TextureDepth* ConvertRenderAPI(RenderAPI api, TextureDepth* original);

		virtual void getPixelData(uint16* pixels) = 0;
		virtual uint getSize() const = 0;

		virtual void setData(const uint16* data) = 0;
	private:
		static std::map<TextureDepth*, TextureDepth*> s_APIChangeMap;
	public:
		static inline void AddRenderAPIChange(TextureDepth* old, TextureDepth* current) { s_APIChangeMap.insert_or_assign(old, current); }
		static inline bool HasRenderAPIChange(TextureDepth* old) { return s_APIChangeMap.find(old) != s_APIChangeMap.end(); }
		static inline TextureDepth* GetRenderAPIChange(TextureDepth* old) { return s_APIChangeMap.at(old); }
		static void FlushRenderAPIChange(RenderAPI prevApi);
	};

} }