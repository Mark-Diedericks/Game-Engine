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
		gedel this->m_Instance;
		this->m_Instance = API::APIVertexArray::Create();
		
		for(VertexBuffer* buffer : m_Buffers)
			this->m_Instance->PushBuffer(buffer);

		return true;
	}

} }