#pragma once
#pragma warning (disable:4251)

#define GE_DEBUG
#define GE_NO_CURSOR NULL

//Determine whether or not to create DLL
#ifdef GE_PLATFORM_WINDOWS
	#ifdef GE_CORE_DLL
		#define GE_API _declspec(dllexport)
	#else
		#define GE_API _declspec(dllexport)
	#endif
#else
	#define GE_API
#endif

#define BIT(x) (1 << x)

#define METHOD_1(x) std::bind(x, this, std::placeholders::_1)
#define METHOD(x) METHOD_1(x)

#ifdef GE_DEBUG
	#define GE_DEBUG_METHOD_V(x) x;
#else
	#define GE_DEBUG_METHOD_V(x) x {}
#endif