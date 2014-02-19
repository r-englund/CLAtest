/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
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
 * Main file authors: Timo Ropinski, Erik Sundén
 *
 *********************************************************************************/

#ifndef IVW_SHADER_H
#define IVW_SHADER_H

#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <modules/opengl/inviwoopengl.h>
#include "shaderobject.h"
#include <map>

namespace inviwo {

class IVW_MODULE_OPENGL_API Shader {

public:
    typedef std::map<GLenum, ShaderObject*> ShaderObjectMap;

    Shader(std::string fragmentFilename, bool linkShader = true);
    Shader(std::string vertexFilename, std::string fragmentFilename, bool linkShader = true);
    Shader(std::string vertexFilename, std::string geometryFilename, std::string fragmentFilename, bool linkShader = true);
    Shader(const char* fragmentFilename, bool linkShader = true);
    Shader(const char* vertexFilename,const char* fragmentFilename, bool linkShader = true);
    Shader(const char* vertexFilename,const char* geometryFilename,const char* fragmentFilename, bool linkShader = true);

    //Takes ownership of shader objects in vector
    Shader(std::vector<ShaderObject*>& shaderObjects, bool linkShader=true);

    virtual ~Shader();

    void link();
    void build();
    void rebuild();

    const ShaderObjectMap* getShaderObjects() { return shaderObjects_; }

    ShaderObject* getVertexShaderObject() { return (*shaderObjects_)[GL_VERTEX_SHADER]; }
    ShaderObject* getGeometryShaderObject() { return (*shaderObjects_)[GL_GEOMETRY_SHADER]; }
    ShaderObject* getFragmentShaderObject() { return (*shaderObjects_)[GL_FRAGMENT_SHADER]; }

    void activate();
    void deactivate();

    void setUniform(const std::string name, GLint value);
    void setUniform(const std::string name, GLfloat value);
    void setUniform(const std::string name, GLfloat* value, int count);
    void setUniform(const std::string name, vec2 value);
    void setUniform(const std::string name, vec3 value);
    void setUniform(const std::string name, vec4 value);
    void setUniform(const std::string name, ivec2 value);
    void setUniform(const std::string name, ivec3 value);
    void setUniform(const std::string name, ivec4 value);
    void setUniform(const std::string name, mat4 value);

private:
    unsigned int id_;

    ShaderObjectMap* shaderObjects_;

    void initialize();
    void linkAndRegister(bool linkShader);
    void deinitialize();

    void createAndAddShader(GLenum, std::string, bool);

    void attachShaderObject(ShaderObject* shaderObject);
    void detachShaderObject(ShaderObject* shaderObject);

    void attachAllShaderObjects();
    void detachAllShaderObject();
};

} // namespace

#endif // IVW_SHADER_H
