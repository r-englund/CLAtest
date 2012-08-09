#ifndef IVW_INVIWOOPENGL_H
#define IVW_INVIWOOPENGL_H

#include "ext/glew/include/GL/glew.h" //TODO: Why is <GL/glew.h> not working
#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>
#include <sstream>
#include <windows.h>

void logGLError(const char* fileName, const char* functionName, int lineNumber);

#if defined(IVW_DEBUG)
    #define LGL_ERROR logGLError(__FILE__, __FUNCTION__, __LINE__)
#else
    #define LGL_ERROR
#endif

#endif // IVW_INVIWOOPENGL_H
