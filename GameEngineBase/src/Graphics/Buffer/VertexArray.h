#pragma once

#include "ge.h"
#include "Common.h"
#include "Backend/API/APIVertexArray.h"

namespace gebase { namespace graphics {

	class GE_API VertexArray
	{
	private:
		API::APIVertexArray* m_Instance;
		VertexArray() { }
	public:
		inline VertexBuffer* getBuffer(uint index = 0) { return m_Instance->getBuffer(index); }
		inline std::vector<gebase::graphics::VertexBuffer*>& getBuffers() { return m_Instance->getBuffers(); }
		inline void PushBuffer(VertexBuffer* buffer) { m_Instance->PushBuffer(buffer); }

		inline void Bind() const { m_Instance->Bind(); }
		inline void Unbind() const { m_Instance->Unbind(); }
		inline void Draw(uint count) const { m_Instance->Draw(count); }

		static VertexArray* Create();
	};

} }