#pragma once

#include "ge.h"
#include "Common.h"
#include "CustomString.h"
#include "Graphics/IRenderAPIDependant.h"

namespace gebase { namespace graphics {

	enum class GE_API TextureType
	{
		TEX2D = 0,
		TEXDEPTH,
		TEXCUBE
	};

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

	class GE_API Texture : public IRenderAPIDependant
	{
	protected:
		Texture(TextureType type, uint loadType) : IRenderAPIDependant(RenderObjectType::Texture, loadType) { m_Type = type; }

		String m_ResourceName;
		TextureType m_Type;

		static TextureWrap s_WrapMode;
		static TextureFilter s_FilterMode;
	public:
		virtual ~Texture() {}

		static Texture* ConvertRenderAPI(RenderAPI api, Texture* original);

		inline const String& getResourceName() const { return m_ResourceName; }
		inline void setResourceName(const String& resourceName) { m_ResourceName = resourceName; }

		virtual void Bind(uint slot = 0) const = 0;
		virtual void Unbind(uint slot = 0) const = 0;

		virtual const String& getName() const = 0;
		virtual const String& getFilepath() const = 0;

		virtual uint getWidth() const = 0;
		virtual uint getHeight() const = 0;

		virtual uint getBitsPerPixel() const = 0;
		virtual uint getMips() const = 0;

		virtual TextureParameters getParameters() const = 0;

		virtual uint getSize() const = 0;

		inline static void setWrap(TextureWrap wrap) { s_WrapMode = wrap; }
		inline static void setFilter(TextureFilter filter) { s_FilterMode = filter; }

		static byte getStrideFromFormat(TextureFormat format);
	};

} }