#include "ge.h"
#include "Common.h"
#include "VertexArray.h"

#include "Graphics/Context.h"
#include "System/Memory.h"

namespace gebase { namespace graphics {

	VertexArray* VertexArray::Create()
	{
		VertexArray* thisVA = genew VertexArray();
		thisVA->m_Instance = API::APIVertexArray::Create();
		return thisVA;
	}

	bool VertexArray::EmployRenderAPI(RenderAPI api)
	{
		std::vector<VertexBuffer*> buffers = this->m_Instance->getBuffers();

		for (uint i = 0; i < buffers.size(); i++)
			if (!buffers[i]->EmployRenderAPI(api))
				return false;

		gedel this->m_Instance;

		this->m_Instance = API::APIVertexArray::Create();
		
		for(VertexBuffer* buffer : buffers)
			this->m_Instance->PushBuffer(buffer);

		return true;
	}

} }