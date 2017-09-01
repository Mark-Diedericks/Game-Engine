#pragma once

#include "Common.h"
#include "Backend/API/APIIndexBuffer.h"
#include "Graphics/IRenderAPIDependant.h"

namespace gebase { namespace graphics {

	class GE_API IndexBuffer : public IRenderAPIDependant
	{
	private:
		uint16* m_Data16;
		uint* m_Data;
		uint m_Count;

		API::APIIndexBuffer* m_Instance;
		IndexBuffer() { }
	public:
		bool EmployRenderAPI(RenderAPI api) override;

		inline uint getCount() const { return m_Instance->getCount(); }

		inline void Bind() const { m_Instance->Bind(); }
		inline void Unbind() const { m_Instance->Unbind(); }

		static IndexBuffer* Create(uint16* data, uint count);
		static IndexBuffer* Create(uint* data, uint count);
	};

} }