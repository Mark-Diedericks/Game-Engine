#pragma once

#include "Common.h"
#include "Types.h"

namespace gebase {

	struct Members;

	class GE_API Timer
	{
	private:
		byte m_Reserved[32];
		Members* m_Members;
	public:
		Timer();
		virtual void Reset();
		virtual float Elapsed();
		virtual float ElapsedMillis();
	};

}