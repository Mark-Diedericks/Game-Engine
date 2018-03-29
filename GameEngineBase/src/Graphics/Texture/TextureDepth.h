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
		static std::vector<TextureDepth*> s_CurrentTextureDepth;
		static std::map<TextureDepth*, TextureDepth*> s_APIChangeMapTextureDepth;
	public:
		static inline void AddRenderAPIChange(TextureDepth* old, TextureDepth* current) { s_APIChangeMapTextureDepth.insert_or_assign(old, current); }
		static inline bool HasRenderAPIChange(TextureDepth* old) { return s_APIChangeMapTextureDepth.find(old) != s_APIChangeMapTextureDepth.end(); }
		static inline TextureDepth* GetRenderAPIChange(TextureDepth* old) { return s_APIChangeMapTextureDepth.at(old); }
		static void PrepareRenderAPIChange(RenderAPI newApi);
		static void FlushRenderAPIChange(RenderAPI prevApi);
	};

} }