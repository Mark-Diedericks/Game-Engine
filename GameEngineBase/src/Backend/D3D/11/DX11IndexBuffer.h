#pragma once

#include "Graphics\Buffer\IndexBuffer.h"
#include "DX11Common.h"

namespace gebase { namespace graphics {

	class DX11IndexBuffer : public IndexBuffer
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

		void getIndexData32(uint* data) override;
		void getIndexData16(uint16* data) override;

		inline uint getCount() const { return m_Count; }
	};

} }