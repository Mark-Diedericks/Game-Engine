#include "ge.h"
#include "APIBufferLayout.h"

namespace gebase { namespace graphics { namespace API {

	APIBufferLayout::APIBufferLayout() : m_Size(0) {}

	void APIBufferLayout::Push(const String& name, uint type, uint size, uint count, bool normalized)
	{
		m_Layout.push_back({ name, type, size, count, size, normalized });
		m_Size += size * count;
	}

} } }