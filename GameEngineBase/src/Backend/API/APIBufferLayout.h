#pragma once

#include "Common.h"
#include "Math/Maths.h"

#include "Graphics/Context.h"

#include "Backend/GL/GLTypes.h"
#include "Backend/D3D/D3DTypes.h"
#include "Backend/VK/VKTypes.h"

namespace gebase { namespace graphics { namespace API {

	enum class GE_API BufferElementType
	{
		UINT_1,
		BYTE_1,
		FLOAT_1,
		FLOAT_2,
		FLOAT_3,
		FLOAT_4,
	};

	struct GE_API BufferElementCount
	{
		uint gl_count;
		uint dx_count;
		uint vk_count;

		uint getCurrentCount()
		{
			switch (graphics::Context::getRenderAPI())
			{
			case graphics::RenderAPI::OPENGL: return gl_count;
			case graphics::RenderAPI::D3D11: return dx_count;
			}

			return 0;
		}
	};

	struct GE_API BufferElement
	{
		String name;
		BufferElementType type;
		BufferElementCount count;
		uint offset;
		bool normalized;
	};

	class APIBufferLayout
	{
	private:
		RenderAPI m_RenderAPI;
		uint m_Size;
		std::vector<BufferElement> m_Layout;

		void Push(const String& name, BufferElementType type, BufferElementCount count, bool normalized);
	public:
		APIBufferLayout();

		void EmployRenderAPI(RenderAPI api);

		inline const std::vector<BufferElement>& getLayout() const { return m_Layout; }
		inline uint getStride() const { return m_Size; }

		template <typename T>
		void Push(const String& name, uint count = 1, bool normalized = false)
		{
			utils::LogUtil::WriteLine("ERROR", "[APIBufferLayout] Push() - Unknown type buffer element type");
		}

		template <>
		void Push<float>(const String& name, uint count, bool normalized)
		{
			Push(name, BufferElementType::FLOAT_1, { count, count, count }, normalized);

			switch (gebase::graphics::Context::getRenderAPI())
			{
			case RenderAPI::OPENGL:
				//Push(name, GL_FLOAT, sizeof(float), count, normalized);
				break;
			case RenderAPI::D3D11:
				//Push(name, DX_TYPE_R32_FLOAT, sizeof(float), count, normalized);
				break;
			}
		}

		template <>
		void Push<uint>(const String& name, uint count, bool normalized)
		{
			Push(name, BufferElementType::UINT_1, { count, count, count }, normalized);

			switch (gebase::graphics::Context::getRenderAPI())
			{
			case RenderAPI::OPENGL:
				//Push(name, GL_UNSIGNED_INT, sizeof(uint), count, normalized);
				break;
			case RenderAPI::D3D11:
				//Push(name, DX_TYPE_R32_UINT, sizeof(uint), count, normalized);
				break;
			}
		}

		template <>
		void Push<byte>(const String& name, uint count, bool normalized)
		{
			Push(name, BufferElementType::BYTE_1, { count, 1, count }, normalized);

			switch (gebase::graphics::Context::getRenderAPI())
			{
			case RenderAPI::OPENGL:
				//Push(name, GL_UNSIGNED_BYTE, sizeof(byte), count, normalized);
				break;
			case RenderAPI::D3D11:
				//Push(name, DX_TYPE_R8G8B8A8_UNORM, sizeof(byte) * 4, 1, normalized);
				break;
			}
		}

		template <>
		void Push<math::Vector2f>(const String& name, uint count, bool normalized)
		{
			Push(name, BufferElementType::FLOAT_2, { 2, count, 2 }, normalized);

			switch (gebase::graphics::Context::getRenderAPI())
			{
			case RenderAPI::OPENGL:
				//Push(name, GL_FLOAT, sizeof(float), 2, normalized);
				break;
			case RenderAPI::D3D11:
				//Push(name, DX_TYPE_R32G32_FLOAT, sizeof(math::Vector2f), count, normalized);
				break;
			}
		}

		template <>
		void Push<math::Vector3f>(const String& name, uint count, bool normalized)
		{
			Push(name, BufferElementType::FLOAT_3, { 3, count, 3 }, normalized);

			switch (gebase::graphics::Context::getRenderAPI())
			{
			case RenderAPI::OPENGL:
				//Push(name, GL_FLOAT, sizeof(float), 3, normalized);
				break;
			case RenderAPI::D3D11:
				//Push(name, DX_TYPE_R32G32B32_FLOAT, sizeof(math::Vector3f), count, normalized);
				break;
			}
		}

		template <>
		void Push<math::Vector4f>(const String& name, uint count, bool normalized)
		{
			Push(name, BufferElementType::FLOAT_4, { 4, count, 4 }, normalized);

			switch (gebase::graphics::Context::getRenderAPI())
			{
			case RenderAPI::OPENGL:
				//Push(name, GL_FLOAT, sizeof(float), 4, normalized);
				break;
			case RenderAPI::D3D11:
				//Push(name, DX_TYPE_R32G32B32A32_FLOAT, sizeof(math::Vector4f), count, normalized);
				break;
			}
		}
	};

} } }