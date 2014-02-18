 /*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Main file author: Erik Sundén
 *
 *********************************************************************************/

#ifndef IVW_GLFORMATS_H
#define IVW_GLFORMATS_H

#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/util/formats.h>
#include <inviwo/core/util/assertion.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API GLFormats{

public:
    struct GLFormat{
        GLint format;
        GLint internalFormat;
        GLenum type;
        GLuint channels;
        GLuint typeSize;
        bool valid;
        GLFormat() : format(0), internalFormat(0), type(0), channels(0), typeSize(0), valid(false) {}
        GLFormat(GLint f, GLint i, GLenum t, GLuint c, GLuint s) : format(f), internalFormat(i), type(t), channels(c), typeSize(s), valid(true) {}
    };

    GLFormats(){
        //1 channel
        glFormatArray_[FLOAT16] = GLFormat(GL_RED, GL_R16F, GL_HALF_FLOAT, 1, 2);
        glFormatArray_[FLOAT32] = GLFormat(GL_RED, GL_R32F, GL_FLOAT, 1, 4);

        glFormatArray_[INT8] = GLFormat(GL_RED, GL_R8, GL_BYTE, 1, 1);
        glFormatArray_[INT12] = GLFormat(GL_RED, GL_R16, GL_SHORT, 1, 2);
        glFormatArray_[INT16] = GLFormat(GL_RED, GL_R16, GL_SHORT, 1, 2);
        glFormatArray_[INT32] = GLFormat(GL_RED, GL_R, GL_INT, 1, 4);

        glFormatArray_[UINT8] = GLFormat(GL_RED, GL_R8, GL_UNSIGNED_BYTE, 1, 1);
        glFormatArray_[UINT12] = GLFormat(GL_RED, GL_R16, GL_UNSIGNED_SHORT, 1, 2);
        glFormatArray_[UINT16] = GLFormat(GL_RED, GL_R16, GL_UNSIGNED_SHORT, 1, 2);
        glFormatArray_[UINT32] = GLFormat(GL_RED, GL_R, GL_UNSIGNED_INT, 1, 4);
        
        //2 channels
        glFormatArray_[Vec2FLOAT16] = GLFormat(GL_RG, GL_RG16F, GL_HALF_FLOAT, 2, 2);
        glFormatArray_[Vec2FLOAT32] = GLFormat(GL_RG, GL_RG32F, GL_FLOAT, 2, 4);

        glFormatArray_[Vec2INT8] =  GLFormat(GL_RG, GL_RG8, GL_BYTE, 2, 1);
        glFormatArray_[Vec2INT12] = GLFormat(GL_RG, GL_RG16, GL_SHORT, 2, 2);
        glFormatArray_[Vec2INT16] = GLFormat(GL_RG, GL_RG16, GL_SHORT, 2, 2);
        glFormatArray_[Vec2INT32] = GLFormat(GL_RG, GL_RG, GL_INT, 2, 4);

        glFormatArray_[Vec2UINT8] = GLFormat(GL_RG, GL_RG8, GL_UNSIGNED_BYTE, 2, 1);
        glFormatArray_[Vec2UINT12] = GLFormat(GL_RG, GL_RG16, GL_UNSIGNED_SHORT, 2, 2);
        glFormatArray_[Vec2UINT16] = GLFormat(GL_RG, GL_RG16, GL_UNSIGNED_SHORT, 2, 2);
        glFormatArray_[Vec2UINT32] = GLFormat(GL_RG, GL_RG, GL_UNSIGNED_INT, 2, 4);

        //3 channels
        glFormatArray_[Vec3FLOAT16] = GLFormat(GL_RGB, GL_RGB16F_ARB, GL_HALF_FLOAT, 3, 2);
        glFormatArray_[Vec3FLOAT32] = GLFormat(GL_RGB, GL_RGB32F_ARB, GL_FLOAT, 3, 4);

        glFormatArray_[Vec3INT8] =  GLFormat(GL_RGB, GL_RGB8, GL_BYTE, 3, 1);
        glFormatArray_[Vec3INT12] = GLFormat(GL_RGB, GL_RGB12, GL_SHORT, 3, 2);
        glFormatArray_[Vec3INT16] = GLFormat(GL_RGB, GL_RGB16, GL_SHORT, 3, 2);
        glFormatArray_[Vec3INT32] = GLFormat(GL_RGB, GL_RGB, GL_INT, 3, 4);

        glFormatArray_[Vec3UINT8] = GLFormat(GL_RGB, GL_RGB8, GL_UNSIGNED_BYTE, 3, 1);
        glFormatArray_[Vec3UINT12] = GLFormat(GL_RGB, GL_RGB12, GL_UNSIGNED_SHORT, 3, 2);
        glFormatArray_[Vec3UINT16] = GLFormat(GL_RGB, GL_RGB16, GL_UNSIGNED_SHORT, 3, 2);
        glFormatArray_[Vec3UINT32] = GLFormat(GL_RGB, GL_RGB, GL_UNSIGNED_INT, 3, 4);

        //4 channels
        glFormatArray_[Vec4FLOAT16] = GLFormat(GL_RGBA, GL_RGBA16F_ARB, GL_HALF_FLOAT, 4, 2);
        glFormatArray_[Vec4FLOAT32] = GLFormat(GL_RGBA, GL_RGBA32F_ARB, GL_FLOAT, 4, 4);

        glFormatArray_[Vec4INT8] =  GLFormat(GL_RGBA, GL_RGBA8, GL_BYTE, 4, 1);
        glFormatArray_[Vec4INT12] = GLFormat(GL_RGBA, GL_RGBA12, GL_SHORT, 4, 2);
        glFormatArray_[Vec4INT16] = GLFormat(GL_RGBA, GL_RGBA16, GL_SHORT, 4, 2);
        glFormatArray_[Vec4INT32] = GLFormat(GL_RGBA, GL_RGBA, GL_INT, 4, 4);

        glFormatArray_[Vec4UINT8] = GLFormat(GL_RGBA, GL_RGBA8, GL_UNSIGNED_BYTE, 4, 1);
        glFormatArray_[Vec4UINT12] = GLFormat(GL_RGBA, GL_RGBA12, GL_UNSIGNED_SHORT, 4, 2);
        glFormatArray_[Vec4UINT16] = GLFormat(GL_RGBA, GL_RGBA16, GL_UNSIGNED_SHORT, 4, 2);
        glFormatArray_[Vec4UINT32] = GLFormat(GL_RGBA, GL_RGBA, GL_UNSIGNED_INT, 4, 4);
    };

    GLFormat getGLFormat(DataFormatId id) const{
        ivwAssert(glFormatArray_[static_cast<int>(id)].valid, "Accessing invalid GLFormat");
        return glFormatArray_[static_cast<int>(id)];   
    };

private:
    GLFormat glFormatArray_[NUMBER_OF_FORMATS];
};

static const GLFormats glFormats_ = GLFormats();
STARTCLANGIGNORE("-Wunused-function")
static const GLFormats* getGLFormats(){
    return &glFormats_;
}
ENDCLANGIGNORE
}

#endif
