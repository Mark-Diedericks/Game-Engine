#include "Pch.h"
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

} }