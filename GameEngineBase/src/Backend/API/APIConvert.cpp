#include "ge.h"
#include "APIConvert.h"

#include "Backend/GL/GLTypes.h"
#include "Backend/D3D/D3DTypes.h"
#include "Backend/VK/VKTypes.h"

#include "APIBufferLayout.h"
#include "Graphics/Context.h"

namespace gebase { namespace graphics { namespace API {

	uint APIConvert::BufferElementSize(API::BufferElementType type)
	{
		switch (graphics::Context::getRenderAPI())
		{
		case graphics::RenderAPI::OPENGL: return BufferElementSizeToGL(type);
		case graphics::RenderAPI::D3D11: return BufferElementSizeToDX(type);
		}

		return 0;
	}	
	
	uint APIConvert::BufferElementType(API::BufferElementType type)
	{
		switch (graphics::Context::getRenderAPI())
		{
		case graphics::RenderAPI::OPENGL: return BufferElementTypeToGL(type);
		case graphics::RenderAPI::D3D11: return BufferElementTypeToDX(type);
		}

		return 0;
	}

	uint APIConvert::BufferElementSizeToGL(API::BufferElementType type)
	{
		switch (type)
		{
		case BufferElementType::UINT_1: return sizeof(uint);
		case BufferElementType::BYTE_1: return sizeof(byte);
		case BufferElementType::FLOAT_1: return sizeof(float);
		case BufferElementType::FLOAT_2: return sizeof(float);
		case BufferElementType::FLOAT_3: return sizeof(float);
		case BufferElementType::FLOAT_4: return sizeof(float);
		}

		return 0;
	}

	uint APIConvert::BufferElementSizeToDX(API::BufferElementType type)
	{
		switch (type)
		{
		case BufferElementType::UINT_1: return sizeof(uint);
		case BufferElementType::BYTE_1: return sizeof(byte) * 4;
		case BufferElementType::FLOAT_1: return sizeof(float);
		case BufferElementType::FLOAT_2: return sizeof(math::Vector2f);
		case BufferElementType::FLOAT_3: return sizeof(math::Vector3f);
		case BufferElementType::FLOAT_4: return sizeof(math::Vector4f);
		}

		return 0;
	}

	uint APIConvert::BufferElementTypeToGL(API::BufferElementType type)
	{
		switch (type)
		{
		case BufferElementType::UINT_1: return GL_UNSIGNED_INT;
		case BufferElementType::BYTE_1: return GL_UNSIGNED_BYTE;
		case BufferElementType::FLOAT_1: return GL_FLOAT;
		case BufferElementType::FLOAT_2: return GL_FLOAT;
		case BufferElementType::FLOAT_3: return GL_FLOAT;
		case BufferElementType::FLOAT_4: return GL_FLOAT;
		}

		return 0;
	}

	uint APIConvert::BufferElementTypeToDX(API::BufferElementType type)
	{
		switch (type)
		{
		case BufferElementType::UINT_1: return DX_TYPE_R32_UINT;
		case BufferElementType::BYTE_1: return DX_TYPE_R8G8B8A8_UNORM;
		case BufferElementType::FLOAT_1: return DX_TYPE_R32_FLOAT;
		case BufferElementType::FLOAT_2: return DX_TYPE_R32G32_FLOAT;
		case BufferElementType::FLOAT_3: return DX_TYPE_R32G32B32_FLOAT;
		case BufferElementType::FLOAT_4: return DX_TYPE_R32G32B32A32_FLOAT;
		}

		return 0;
	}

} } }