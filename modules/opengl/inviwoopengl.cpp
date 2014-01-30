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
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#include <modules/opengl/inviwoopengl.h>
#include <inviwo/core/util/logcentral.h>

void LogGLError(const char* fileName, const char* functionName, int lineNumber) {
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        const GLubyte* errorString = gluErrorString(err);
        std::ostringstream errorMessage;
        errorMessage << "OpenGL Error (" << fileName << ", " << functionName << ", Ln " << lineNumber << "): ";
        errorMessage << (errorString ? (const char*)errorString : "undefined");
        inviwo::LogCentral::instance()->log("OpenGL", inviwo::Error, __FILE__, __FUNCTION__, __LINE__, (errorMessage.str()));
    }
}