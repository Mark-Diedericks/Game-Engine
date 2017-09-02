#pragma once

#include "Backend/API/APIIndexBuffer.h"
#include "DX11Common.h"

namespace gebase { namespace graphics { namespace API {

	class DX11IndexBuffer : public APIIndexBuffer
	{
	private:
		ID3D11Buffer* m_Handle;
		uint m_Count;
	public:
		DX11IndexBuffer(uint16* data, uint count);
		DX11IndexBuffer(uint* data, uint count);
		~DX11IndexBuffer();

		void Bind() const;
		void Unbind() const;

		uint* getIndexData32() override;
		uint16* getIndexData16() override;

		inline uint getCount() const { return m_Count; }
	};

} } }