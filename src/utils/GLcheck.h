#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <utils/Log.h>

const GLubyte* gluErrorString(GLenum errorCode);
void PrintOpenGLErrors(char const* const Function, char const* const File, int const Line) {
	GLenum Error = glGetError();
	if (Error != GL_NO_ERROR)
		LERROR("OpenGL Error in {0} at line {1} calling function {2}: '{3}'", File, Line, Function, Error);
}

void PrintShaderInfoLog(GLint const Shader) {
	int InfoLogLength = 0;
	int CharsWritten = 0;

	glGetShaderiv(Shader, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		GLchar* InfoLog = new GLchar[InfoLogLength];
		glGetShaderInfoLog(Shader, InfoLogLength, &CharsWritten, InfoLog);
		LINFO("Shader Info Log:");
		LINFO(InfoLog);
		delete[] InfoLog;
	}
}

#ifdef NDEBUG
	#define CheckCall(x)
	#define CheckShader(x)
#else
	#define CheckCall(x) { (x); PrintOpenGLErrors(#x, __FILE__, __LINE__); };
	#define CheckShader(x) { PrintShaderInfoLog(x); };
#endif