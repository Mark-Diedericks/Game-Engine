#pragma once

#include "Common.h"
#include "Math/Maths.h"

#include "Graphics/Context.h"

#include "Backend/GL/GLTypes.h"
#include "Backend/D3D/D3DTypes.h"
#include "Backend/VK/VKTypes.h"

namespace gebase { namespace graphics { namespace API {

	struct GE_API BufferElement
	{
		String name;
		uint type;
		uint size;
		uint count;
		uint offset;
		bool normalized;
	};

	class APIBufferLayout
	{
	private:
		uint m_Size;
		std::vector<BufferElement> m_Layout;

		void Push(const String& name, uint type, uint size, uint count, bool normalized);
	public:
		APIBufferLayout();

		inline const std::vector<BufferElement>& getLayout() const { return m_Layout; }
		inline uint getStride() const { return m_Size; }

		template <typename T>
		void Push(const String& name, uint count = 1, bool normalized = false)
		{
			std::cout << "Error, unknown type buffer element type" << std::endl;
		}

		template <>
		void Push<float>(const String& name, uint count, bool normalized)
		{
			switch (gebase::graphics::Context::getRenderAPI())
			{
			case RenderAPI::OPENGL:
				Push(name, GL_FLOAT, sizeof(float), count, normalized);
				break;
			case RenderAPI::VULKAN:
				Push(name, 0, sizeof(float), count, normalized);
				break;
			case RenderAPI::D3D11:
				Push(name, DX_TYPE_R32_FLOAT, sizeof(float), count, normalized);
				break;
			case RenderAPI::D3D12:
				Push(name, DX_TYPE_R32_FLOAT, sizeof(float), count, normalized);
				break;
			}
		}

		template <>
		void Push<uint>(const String& name, uint count, bool normalized)
		{
			switch (gebase::graphics::Context::getRenderAPI())
			{
			case RenderAPI::OPENGL:
				Push(name, GL_UNSIGNED_INT, sizeof(uint), count, normalized);
				break;
			case RenderAPI::VULKAN:
				Push(name, 0, sizeof(uint), count, normalized);
				break;
			case RenderAPI::D3D11:
				Push(name, DX_TYPE_R32_UINT, sizeof(uint), count, normalized);
				break;
			case RenderAPI::D3D12:
				Push(name, DX_TYPE_R32_UINT, sizeof(uint), count, normalized);
				break;
			}
		}

		template <>
		void Push<byte>(const String& name, uint count, bool normalized)
		{
			switch (gebase::graphics::Context::getRenderAPI())
			{
			case RenderAPI::OPENGL:
				Push(name, GL_UNSIGNED_BYTE, sizeof(byte), count, normalized);
				break;
			case RenderAPI::VULKAN:
				Push(name, 0, sizeof(byte) * 4, 1, normalized);
				break;
			case RenderAPI::D3D11:
				Push(name, DX_TYPE_R8G8B8A8_UNORM, sizeof(byte), count, normalized);
				break;
			case RenderAPI::D3D12:
				Push(name, DX_TYPE_R8G8B8A8_UNORM, sizeof(byte), count, normalized);
				break;
			}
		}

		template <>
		void Push<math::Vector2f>(const String& name, uint count, bool normalized)
		{
			switch (gebase::graphics::Context::getRenderAPI())
			{
			case RenderAPI::OPENGL:
				Push(name, GL_FLOAT, sizeof(math::Vector2f), 2, normalized);
				break;
			case RenderAPI::VULKAN:
				Push(name, 0, sizeof(math::Vector2f), count, normalized);
				break;
			case RenderAPI::D3D11:
				Push(name, DX_TYPE_R32G32_FLOAT, sizeof(math::Vector2f), count, normalized);
				break;
			case RenderAPI::D3D12:
				Push(name, DX_TYPE_R32G32_FLOAT, sizeof(math::Vector2f), count, normalized);
				break;
			}
		}

		template <>
		void Push<math::Vector3f>(const String& name, uint count, bool normalized)
		{
			switch (gebase::graphics::Context::getRenderAPI())
			{
			case RenderAPI::OPENGL:
				Push(name, GL_FLOAT, sizeof(math::Vector3f), 3, normalized);
				break;
			case RenderAPI::VULKAN:
				Push(name, 0, sizeof(math::Vector3f), count, normalized);
				break;
			case RenderAPI::D3D11:
				Push(name, DX_TYPE_R32G32B32_FLOAT, sizeof(math::Vector3f), count, normalized);
				break;
			case RenderAPI::D3D12:
				Push(name, DX_TYPE_R32G32B32_FLOAT, sizeof(math::Vector3f), count, normalized);
				break;
			}
		}

		template <>
		void Push<math::Vector4f>(const String& name, uint count, bool normalized)
		{
			switch (gebase::graphics::Context::getRenderAPI())
			{
			case RenderAPI::OPENGL:
				Push(name, GL_FLOAT, sizeof(math::Vector4f), 4, normalized);
				break;
			case RenderAPI::VULKAN:
				Push(name, 0, sizeof(math::Vector4f), count, normalized);
				break;
			case RenderAPI::D3D11:
				Push(name, DX_TYPE_R32G32B32A32_FLOAT, sizeof(math::Vector4f), count, normalized);
				break;
			case RenderAPI::D3D12:
				Push(name, DX_TYPE_R32G32B32A32_FLOAT, sizeof(math::Vector4f), count, normalized);
				break;
			}
		}
	};

} } }