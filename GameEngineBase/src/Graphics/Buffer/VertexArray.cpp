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
		if (current == api)
			return true;

		current = api;

		std::vector<VertexBuffer*> buffers(m_Instance->getBuffers());

		for (uint i = 0; i < buffers.size(); i++)
			if (!buffers[i]->EmployRenderAPI(api))
				return false;

		API::APIVertexArray* inst = API::APIVertexArray::Create();

		for (uint i = 0; i < buffers.size(); i++)
			inst->PushBuffer(buffers[i]);

		gedel this->m_Instance;
		this->m_Instance = inst;

		return true;
	}

} }