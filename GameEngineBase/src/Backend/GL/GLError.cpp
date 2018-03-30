#include "ge.h"
#include "Common.h"
#include "CustomString.h"
#include "Utils\LogUtil.h"
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
			utils::LogUtil::WriteLine("ERROR", "[OpenGL Error] - " + std::to_string((byte)glGetString(error)) + "(" + std::to_string(error) + "): " + (String)function + " in file " + (String)file + " at line " + std::to_string(line));
			return false;
		}

		return true;
	}
}