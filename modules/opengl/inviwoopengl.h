/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#ifndef IVW_INVIWOOPENGL_H
#define IVW_INVIWOOPENGL_H

#include <modules/opengl/openglmoduledefine.h>

#ifdef WIN32
#include <windows.h>
#endif

#include <modules/opengl/ext/glew/include/GL/glew.h> //TODO: Why is <GL/glew.h> not working

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif // __APPLE__
#include <iostream>
#include <sstream>

#include "glformats.h"

IVW_MODULE_OPENGL_API void LogGLError(const char* fileName, const char* functionName, int lineNumber);

#if defined(_DEBUG)
    #define LGL_ERROR LogGLError(__FILE__, __FUNCTION__, __LINE__)
    #define LGL_ERROR_SUPPRESS glGetError()
#else
    #define LGL_ERROR
    #define LGL_ERROR_SUPPRESS
#endif

#endif // IVW_INVIWOOPENGL_H
