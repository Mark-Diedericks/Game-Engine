#include "Pch.h"
#include "Common.h"

#include "GLCommon.h"

namespace gebase {

	GLenum GLCheckError()
	{
		return glGetError();
	}

	bool GLLogCall(const char* function, const char* file, int32 line)
	{
		GLenum error = GLCheckError();

		if (error != GL_NO_ERROR)
		{
			std::cout << "[OpenGL Error]" << std::to_string(error) << ": " << function << " in file " << file << " at line " << std::to_string(line) << std::endl;
			return false;
		}

		return true;
	}
}