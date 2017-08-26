#pragma once

#include "Backend/API/APIVertexArray.h"
#include "GLCommon.h"

namespace gebase { namespace graphics { namespace API {

	class GLVertexArray : public APIVertexArray
	{
	private:
		uint m_Handle;
		std::vector<gebase::graphics::VertexBuffer*> m_Buffers;
	public:
		GLVertexArray();
		~GLVertexArray();

		void Bind() const override;
		void Unbind() const override;
		void Draw(uint count) const override;

		void PushBuffer(gebase::graphics::VertexBuffer* buffer);
		inline gebase::graphics::VertexBuffer* getBuffer(uint index = 0) override { return m_Buffers[index]; }
		inline std::vector<gebase::graphics::VertexBuffer*>& getBuffers() override { return m_Buffers; }
	};

} } }