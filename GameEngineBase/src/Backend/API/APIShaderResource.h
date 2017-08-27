#pragma once

#include "Common.h"
#include "CustomString.h"

namespace gebase { namespace graphics { namespace API {

	class ShaderResourceDeclaration
	{
	public:
		virtual const String& getName() const = 0;
		virtual uint getRegister() const = 0;
		virtual uint getCount() const = 0;
	};

	typedef std::vector<ShaderResourceDeclaration*> ShaderResourceList;

} } }