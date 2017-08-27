#pragma once

#include "Common.h"
#include "String.h"

namespace gebase { namespace entity {
	class Entity;
} }

namespace gebase { namespace entity { namespace component {

	struct GE_API ComponentType
	{
		String name;
	};

	class GE_API Component
	{
	protected:
		Entity* m_Entity;
	public:
		virtual Entity* getEntity() { return m_Entity; }
		virtual ComponentType* getType() const { return nullptr; }
	};

} } }