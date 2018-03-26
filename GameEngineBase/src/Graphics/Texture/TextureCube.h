#pragma once

#include "Texture.h"

namespace gebase { namespace graphics {

	class GE_API TextureCube : public Texture
	{
	protected:
		enum class InputFormat
		{
			VERTICAL_CROSS,
			HORIZONTAL_CROSS
		};

		TextureCube(uint loadType) : Texture(TextureType::TEXCUBE, loadType) {}
	public:
		static TextureCube* CreateFromFile(const String& filepath);
		static TextureCube* CreateFromFiles(const String* files);
		static TextureCube* CreateFromVerticalCross(const String* files, int32 mips);

		static TextureCube* CreateFromFile(const String& name, const byte* pixels, uint width, uint height, uint bits);
		static TextureCube* CreateFromFiles(const String& name, const byte** sides, uint width, uint height, uint bits);
		static TextureCube* CreateFromVerticalCross(const String& name, const byte** sides, int32 mips, uint* width, uint* height, uint bits);
		static TextureCube* CreateFromVerticalCross(const String& name, const byte*** faces, int32 mips, uint* faceWidths, uint* faceHeights, uint bits);

		static TextureCube* ConvertRenderAPI(RenderAPI api, TextureCube* original);

		virtual uint getWidth(uint index) const = 0;
		virtual uint getHeight(uint index) const = 0;

		virtual void getPixelData(byte*** pixels) = 0;
		virtual uint getSize() const = 0;
	private:
		static std::map<TextureCube*, TextureCube*> s_APIChangeMap;
	public:
		static inline void AddRenderAPIChange(TextureCube* old, TextureCube* current) { s_APIChangeMap.insert_or_assign(old, current); }
		static inline bool HasRenderAPIChange(TextureCube* old) { return s_APIChangeMap.find(old) != s_APIChangeMap.end(); }
		static inline TextureCube* GetRenderAPIChange(TextureCube* old) { return s_APIChangeMap.at(old); }
		static void FlushRenderAPIChange(RenderAPI prevApi);
	};

} } 