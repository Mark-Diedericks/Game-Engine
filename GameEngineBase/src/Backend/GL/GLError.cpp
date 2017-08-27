#include "ge.h"
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
			std::cout << "[OpenGL Error]" << (const char*)glGetString(error) << ": " << function << " in file " << file << " at line " << StringFormat::Float(line, 0).c_str() << std::endl;
			return false;
		}

		return true;
	}
}