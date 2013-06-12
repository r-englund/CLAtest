#ifndef IVW_SHADER_H
#define IVW_SHADER_H

#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <modules/opengl/inviwoopengl.h>
#include "shaderobject.h"

namespace inviwo {

class IVW_MODULE_OPENGL_API Shader {

public:
    Shader(std::string fragmentFilename, bool linkShader=true);
    Shader(std::string vertexFilename, std::string fragmentFilename, bool linkShader=true);
    virtual ~Shader();

    void link();
    void build();
    void rebuild();

    ShaderObject* getVertexShaderObject() { return vertexShaderObject_; }
    ShaderObject* getFragmentShaderObject() { return fragmentShaderObject_; }

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

    std::string vertexFilename_;
    std::string fragmentFilename_;
    ShaderObject* vertexShaderObject_;
    ShaderObject* fragmentShaderObject_;

    void initialize(bool linkShader);
    void deinitialize();

    void attachShaderObject(ShaderObject* shaderObject);
    void detachShaderObject(ShaderObject* shaderObject);
};

} // namespace

#endif // IVW_SHADER_H
