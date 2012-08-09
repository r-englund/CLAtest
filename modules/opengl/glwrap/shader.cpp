#include "shader.h"

namespace inviwo {

    Shader::Shader(std::string fragmentFilename) :
        vertexFilename_("img_identity.vert"),
        fragmentFilename_(fragmentFilename) {
        initialize();
    }

    Shader::Shader(std::string vertexFilename, std::string fragmentFilename) :
        vertexFilename_(vertexFilename),
        fragmentFilename_(fragmentFilename) {
        initialize();
    }

    Shader::~Shader() {
        glDeleteProgram(id_);
        LGL_ERROR;
    }

    void Shader::attachShaderObject(ShaderObject* shaderObject) {
        glAttachShader(id_, shaderObject->getID());
        LGL_ERROR;
    }

    void Shader::link() {
        glLinkProgram(id_);
        LGL_ERROR;
    }

    void Shader::initialize() {
        id_ = glCreateProgram();
        LGL_ERROR;
        vertexShaderObject_ = new ShaderObject(GL_VERTEX_SHADER , vertexFilename_);
        fragmentShaderObject_ = new ShaderObject(GL_FRAGMENT_SHADER , fragmentFilename_);
        vertexShaderObject_->initialize();
        fragmentShaderObject_->initialize();
        attachShaderObject(vertexShaderObject_);
        attachShaderObject(fragmentShaderObject_);
        link();
    }

    void Shader::deinitialize() {}

    void Shader::activate() {
        glUseProgram(id_);
        LGL_ERROR;
    }

    void Shader::deactivate() {
        glUseProgram(0);
        LGL_ERROR;
    }

    void Shader::setUniform(const std::string name, GLint value) {
        GLint uniformLocation = glGetUniformLocation(id_, name.c_str());
        if (uniformLocation != -1) glUniform1i(uniformLocation, value);
        else std::cout << "error setting uniform" << std::endl;
    }

} // namespace
