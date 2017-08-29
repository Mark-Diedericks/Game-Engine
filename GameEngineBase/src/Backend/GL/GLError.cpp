#include "ge.h"
#include "Common.h"
#include "CustomString.h"

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
			std::cout << "[OpenGL Error]" << (int)(error) << ": " << function << " in file " << file << " at line " << (int)line << std::endl;
			return false;
		}

		return true;
	}
}