#pragma once

#include "Graphics\Buffer\VertexArray.h"

namespace gebase { namespace graphics {

	class DX11VertexArray : public VertexArray
	{
	private:
		uint m_Handle;
		std::vector<VertexBuffer*> m_Buffers;

		void Draw(uint count, uint index) const;
	public:
		DX11VertexArray();
		~DX11VertexArray();

		inline VertexBuffer* getBuffer(uint index = 0) override { return m_Buffers[index]; }
		inline std::vector<VertexBuffer*>& getBuffers() override { return m_Buffers; }
		void PushBuffer(VertexBuffer* buffer) override;

		void Bind() const override;
		void Unbind() const override;
		void Draw(uint count) const override;
	};

} }