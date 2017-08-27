#pragma once

#include "Backend/API/APIVertexArray.h"

namespace gebase { namespace graphics { namespace API {

	class DX11VertexArray : public APIVertexArray
	{
	private:
		uint m_Handle;
		std::vector<gebase::graphics::VertexBuffer*> m_Buffers;

		void Draw(uint count, uint index) const;
	public:
		DX11VertexArray();
		~DX11VertexArray();

		inline gebase::graphics::VertexBuffer* getBuffer(uint index = 0) override { return m_Buffers[index]; }
		inline std::vector<gebase::graphics::VertexBuffer*>& getBuffers() override { return m_Buffers; }
		void PushBuffer(gebase::graphics::VertexBuffer* buffer) override;

		void Bind() const override;
		void Unbind() const override;
		void Draw(uint count) const override;
	};

} } }