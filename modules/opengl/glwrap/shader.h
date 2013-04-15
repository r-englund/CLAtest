#ifndef IVW_SHADER_H
#define IVW_SHADER_H

#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/inviwo.h>
#include <modules/opengl/inviwoopengl.h>
#include "shaderobject.h"

namespace inviwo {

class IVW_MODULE_OPENGL_API Shader {

public:
    Shader(std::string fragmentFilename);
    Shader(std::string vertexFilename, std::string fragmentFilename);
    virtual ~Shader();

    void link();

    ShaderObject* getVertexShaderObject() { return vertexShaderObject_; }
    ShaderObject* getFragmentShaderObject() { return fragmentShaderObject_; }

    void activate();
    void deactivate();

    void setUniform(const std::string name, GLint value);
    void setUniform(const std::string name, GLfloat value);
    void setUniform(const std::string name, vec2 value);
    void setUniform(const std::string name, vec3 value);
    void setUniform(const std::string name, vec4 value);
    void setUniform(const std::string name, mat4 value);

private:
    unsigned int id_;

    std::string vertexFilename_;
    std::string fragmentFilename_;
    ShaderObject* vertexShaderObject_;
    ShaderObject* fragmentShaderObject_;

    void initialize();
    void deinitialize();

    void attachShaderObject(ShaderObject* shaderObject);
    void detachShaderObject(ShaderObject* shaderObject);
};

} // namespace

#endif // IVW_SHADER_H
