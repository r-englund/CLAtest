#include "shader.h"

namespace inviwo {

const std::string Shader::logSource_ = "Shader";

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
    else
        LogWarn("Unable to set uniform " + name + " in shader " + vertexFilename_ + "/" + fragmentFilename_);
}

void Shader::setUniform(const std::string name, GLfloat value) {
    GLint uniformLocation = glGetUniformLocation(id_, name.c_str());
    if (uniformLocation != -1) glUniform1f(uniformLocation, value);
    else
        LogWarn("Unable to set uniform " + name + " in shader " + vertexFilename_ + "/" + fragmentFilename_);
}

void Shader::setUniform(const std::string name, vec2 value) {
    GLint uniformLocation = glGetUniformLocation(id_, name.c_str());
    GLfloat* glvalues = new GLfloat[2];
    glvalues[0] = value[0];
    glvalues[1] = value[1];
    if (uniformLocation != -1) glUniform2fv(uniformLocation, 1, glvalues);
    else
        LogWarn("Unable to set uniform " + name + " in shader " + vertexFilename_ + "/" + fragmentFilename_);
}
} // namespace
