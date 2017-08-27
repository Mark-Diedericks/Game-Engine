#pragma once

#include "ge.h"
#include "Common.h"
#include "APIVertexBuffer.h"
#include "Graphics/Buffer/VertexBuffer.h"

namespace gebase { namespace graphics { namespace API {

	class APIVertexArray
	{
	public:
		virtual gebase::graphics::VertexBuffer* getBuffer(uint index = 0) = 0;
		virtual std::vector<gebase::graphics::VertexBuffer*>& getBuffers() = 0;
		virtual void PushBuffer(gebase::graphics::VertexBuffer* buffer) = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void Draw(uint count) const = 0;

		static APIVertexArray* Create();
	};

} } }