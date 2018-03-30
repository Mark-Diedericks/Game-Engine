#include "ge.h"
#include "ConversionUtil.h"

#include "Backend/GL/GLTypes.h"
#include "Backend/D3D/D3DTypes.h"
#include "Backend/VK/VKTypes.h"

#include "Graphics/Buffer/BufferLayout.h"
#include "Graphics/Context/Context.h"

namespace gebase { namespace graphics {

	uint ConversionUtil::GetBufferElementSize(BufferElementType type, graphics::RenderAPI api)
	{
		switch (api)
		{
		case graphics::RenderAPI::OPENGL: return BufferElementSizeToGL(type);
		case graphics::RenderAPI::D3D11: return BufferElementSizeToDX(type);
		}

		return 0;
	}	
	
	uint ConversionUtil::GetBufferElementType(BufferElementType type, graphics::RenderAPI api)
	{
		switch (api)
		{
		case graphics::RenderAPI::OPENGL: return BufferElementTypeToGL(type);
		case graphics::RenderAPI::D3D11: return BufferElementTypeToDX(type);
		}

		return 0;
	}

	uint ConversionUtil::BufferElementSizeToGL(BufferElementType type)
	{
		switch (type)
		{
		case graphics::BufferElementType::UINT_1: return sizeof(uint);
		case graphics::BufferElementType::BYTE_1: return sizeof(byte);
		case graphics::BufferElementType::FLOAT_1: return sizeof(float);
		case graphics::BufferElementType::FLOAT_2: return sizeof(float);
		case graphics::BufferElementType::FLOAT_3: return sizeof(float);
		case graphics::BufferElementType::FLOAT_4: return sizeof(float);
		}

		return 0;
	}

	uint ConversionUtil::BufferElementSizeToDX(BufferElementType type)
	{
		switch (type)
		{
		case graphics::BufferElementType::UINT_1: return sizeof(uint);
		case graphics::BufferElementType::BYTE_1: return sizeof(byte) * 4;
		case graphics::BufferElementType::FLOAT_1: return sizeof(float);
		case graphics::BufferElementType::FLOAT_2: return sizeof(math::Vector2f);
		case graphics::BufferElementType::FLOAT_3: return sizeof(math::Vector3f);
		case graphics::BufferElementType::FLOAT_4: return sizeof(math::Vector4f);
		}

		return 0;
	}

	uint ConversionUtil::BufferElementTypeToGL(BufferElementType type)
	{
		switch (type)
		{
		case graphics::BufferElementType::UINT_1: return GL_UNSIGNED_INT;
		case graphics::BufferElementType::BYTE_1: return GL_UNSIGNED_BYTE;
		case graphics::BufferElementType::FLOAT_1: return GL_FLOAT;
		case graphics::BufferElementType::FLOAT_2: return GL_FLOAT;
		case graphics::BufferElementType::FLOAT_3: return GL_FLOAT;
		case graphics::BufferElementType::FLOAT_4: return GL_FLOAT;
		}

		return 0;
	}

	uint ConversionUtil::BufferElementTypeToDX(BufferElementType type)
	{
		switch (type)
		{
		case graphics::BufferElementType::UINT_1: return DX_TYPE_R32_UINT;
		case graphics::BufferElementType::BYTE_1: return DX_TYPE_R8G8B8A8_UNORM;
		case graphics::BufferElementType::FLOAT_1: return DX_TYPE_R32_FLOAT;
		case graphics::BufferElementType::FLOAT_2: return DX_TYPE_R32G32_FLOAT;
		case graphics::BufferElementType::FLOAT_3: return DX_TYPE_R32G32B32_FLOAT;
		case graphics::BufferElementType::FLOAT_4: return DX_TYPE_R32G32B32A32_FLOAT;
		}

		return 0;
	}

} }