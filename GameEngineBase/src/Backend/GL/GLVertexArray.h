#pragma once

#include "Graphics\Buffer\VertexArray.h"
#include "GLCommon.h"

namespace gebase { namespace graphics {

	class GLVertexArray : public VertexArray
	{
	private:
		uint m_Handle;
		std::vector<VertexBuffer*> m_Buffers;
	public:
		GLVertexArray();
		~GLVertexArray();

		void Bind() const override;
		void Unbind() const override;
		void Draw(uint count) const override;

		void PushBuffer(VertexBuffer* buffer);
		inline VertexBuffer* getBuffer(uint index = 0) override { return m_Buffers[index]; }
		inline std::vector<VertexBuffer*>& getBuffers() override { return m_Buffers; }
	};

} }