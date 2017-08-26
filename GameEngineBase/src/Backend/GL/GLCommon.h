#pragma once

#include "Common.h"
#include <GL\glew.h>

namespace gebase {

	extern GLenum GLCheckError();
	extern bool GLLogCall(const char* function, const char* file, int32 line);

}

#ifdef GE_DEBUG
	#define GLCall(x) gebase::GLCheckError();\
		x;\
		if (!gebase::GLLogCall(#x, __FILE__, __LINE__)) __debugbreak();
#else
	#define GLCall(x) x
#endif