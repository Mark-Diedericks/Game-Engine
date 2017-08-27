#pragma once
#pragma warning (disable:4251)

#ifdef GE_PLATFORM_WINDOWS
	#ifdef GE_CORE_DLL
		#define GE_API __declspec(dllexport)
	#else
		#define GE_API __declspec(dllimport)
	#endif
#else
	#define GE_API
#endif

// Common defines
#define BIT(x) (1 << x)

#define METHOD_1(x) std::bind(x, this, std::placeholders::_1)
#define METHOD(x) METHOD_1(x)

#ifdef GE_DEBUG
	#define GE_DEBUG_METHOD_V(x) x;
#else
	#define GE_DEBUG_METHOD_V(x) x {}
#endif