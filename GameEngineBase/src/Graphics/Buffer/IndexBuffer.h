#pragma once

#include "Common.h"
#include "Backend/API/APIIndexBuffer.h"
#include "Graphics/IRenderAPIDependant.h"

namespace gebase { namespace graphics {

	class GE_API IndexBuffer : public IRenderAPIDependant
	{
	private:
		API::APIIndexBuffer* m_Instance;

		uint m_Count;
		uint m_Type;

		IndexBuffer() : IRenderAPIDependant(RenderObjectType::Buffer) { }
	public:
		bool EmployRenderAPI(RenderAPI api) override;

		inline uint getCount() const { return m_Instance->getCount(); }

		inline void Bind() const { m_Instance->Bind(); }
		inline void Unbind() const { m_Instance->Unbind(); }

		static IndexBuffer* Create(uint16* data, uint count);
		static IndexBuffer* Create(uint* data, uint count);
	};

} }