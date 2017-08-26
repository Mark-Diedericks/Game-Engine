#pragma once

#include "Pch.h"
#include "Common.h"
#include "String.h"

namespace gebase { namespace graphics { namespace API {

			enum class GE_API TextureWrap
			{
				NONE = 0,
				REPEAT,
				CLAMP,
				MIRRORED_REPEAT,
				CLAMP_TO_EDGE,
				CLAMP_TO_BORDER
			};

			enum class GE_API TextureFilter
			{
				NONE = 0,
				LINEAR,
				NEAREST
			};

			enum class GE_API TextureFormat
			{
				NONE = 0,
				RGB,
				RGBA,
				LUMINANCE,
				LUMINANCE_ALPHA
			};

			struct GE_API TextureParameters
			{
				TextureFormat format;
				TextureFilter filter;
				TextureWrap wrap;

				TextureParameters() : format(TextureFormat::RGBA), filter(TextureFilter::LINEAR), wrap(TextureWrap::CLAMP) {}
				TextureParameters(TextureFormat format) : format(format), filter(TextureFilter::LINEAR), wrap(TextureWrap::CLAMP) {}
				TextureParameters(TextureFormat format, TextureFilter filter) : format(format), filter(filter), wrap(TextureWrap::CLAMP) {}
				TextureParameters(TextureFormat format, TextureFilter filter, TextureWrap wrap) : format(format), filter(filter), wrap(wrap) {}
			};

			struct GE_API TextureLoadOptions
			{
				bool flipX;
				bool flipY;

				TextureLoadOptions() : flipX(false), flipY(false) {}
				TextureLoadOptions(bool flipX, bool flipY) : flipX(flipX), flipY(flipY) {}
			};

			class APITexture
			{
			protected:
				static TextureWrap s_WrapMode;
				static TextureFilter s_FilterMode;
			public:
				virtual ~APITexture() {}

				virtual void Bind(uint slot = 0) const = 0;
				virtual void Unbind(uint slot = 0) const = 0;

				virtual const String& getName() const = 0;
				virtual const String& getFilepath() const = 0;

				virtual uint getWidth() const = 0;
				virtual uint getHeight() const = 0;

				inline static void setWrap(TextureWrap wrap) { s_WrapMode = wrap; }
				inline static void setFilter(TextureFilter filter) { s_FilterMode = filter; }

				static byte getStrideFromFormat(TextureFormat format);
			};

} } }