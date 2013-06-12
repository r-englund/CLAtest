#include "shader.h"

#include <modules/opengl/glwrap/shadermanager.h>

namespace inviwo {

Shader::Shader(std::string fragmentFilename, bool linkShader) :
    vertexFilename_("img_identity.vert"),
    fragmentFilename_(fragmentFilename)
{
    initialize(linkShader);
}

Shader::Shader(std::string vertexFilename, std::string fragmentFilename, bool linkShader) :
    vertexFilename_(vertexFilename),
    fragmentFilename_(fragmentFilename)
{
    initialize(linkShader);
}

Shader::~Shader() {
    deinitialize();
}


void Shader::initialize(bool linkShader) {
    id_ = glCreateProgram();
    LGL_ERROR;
    vertexShaderObject_ = new ShaderObject(GL_VERTEX_SHADER , vertexFilename_);
    fragmentShaderObject_ = new ShaderObject(GL_FRAGMENT_SHADER , fragmentFilename_);
    attachShaderObject(vertexShaderObject_);
    attachShaderObject(fragmentShaderObject_);
    if (linkShader) link();
    ShaderManager::getRef().registerShader(this);
}

void Shader::deinitialize() {
    ShaderManager::getRef().unregisterShader(this);
    detachShaderObject(vertexShaderObject_);
    detachShaderObject(fragmentShaderObject_);
    delete vertexShaderObject_;
    delete fragmentShaderObject_;
    glDeleteProgram(id_);
    LGL_ERROR;
}


void Shader::attachShaderObject(ShaderObject* shaderObject) {
    glAttachShader(id_, shaderObject->getID());
    LGL_ERROR;
}

void Shader::detachShaderObject(ShaderObject* shaderObject) {
    glDetachShader(id_, shaderObject->getID());
    LGL_ERROR;
}


void Shader::link() {
    glLinkProgram(id_);
    LGL_ERROR;
}

void Shader::build() {
    vertexShaderObject_->build();
    fragmentShaderObject_->build();
    link();
}

void Shader::rebuild() {
    vertexShaderObject_->rebuild();
    fragmentShaderObject_->rebuild();
    link();
}


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
    //else
        //LogWarn("Unable to set uniform " + name + " in shader " + vertexFilename_ + "/" + fragmentFilename_);
}

void Shader::setUniform(const std::string name, GLfloat value) {
    GLint uniformLocation = glGetUniformLocation(id_, name.c_str());
    if (uniformLocation != -1) glUniform1f(uniformLocation, value);
    //else
        //LogWarn("Unable to set uniform " + name + " in shader " + vertexFilename_ + "/" + fragmentFilename_);
}

void Shader::setUniform(const std::string name, vec2 value) {
    GLint uniformLocation = glGetUniformLocation(id_, name.c_str());
    GLfloat* glvalues = new GLfloat[2];
    glvalues[0] = value[0];
    glvalues[1] = value[1];
    if (uniformLocation != -1) glUniform2fv(uniformLocation, 1, glm::value_ptr(value));
    //else
        //LogWarn("Unable to set uniform " + name + " in shader " + vertexFilename_ + "/" + fragmentFilename_);
}

void Shader::setUniform(const std::string name, vec3 value) {
    GLint uniformLocation = glGetUniformLocation(id_, name.c_str());
    GLfloat* glvalues = new GLfloat[3];
    glvalues[0] = value[0];
    glvalues[1] = value[1];
    glvalues[2] = value[2];
    if (uniformLocation != -1) glUniform3fv(uniformLocation, 1, glm::value_ptr(value));
    //else
        //LogWarn("Unable to set uniform " + name + " in shader " + vertexFilename_ + "/" + fragmentFilename_);
}

void Shader::setUniform(const std::string name, vec4 value) {
    GLint uniformLocation = glGetUniformLocation(id_, name.c_str());
    GLfloat* glvalues = new GLfloat[4];
    glvalues[0] = value[0];
    glvalues[1] = value[1];
    glvalues[2] = value[2];
    glvalues[3] = value[3];
    if (uniformLocation != -1) glUniform4fv(uniformLocation, 1, glm::value_ptr(value));
    //else
        //LogWarn("Unable to set uniform " + name + " in shader " + vertexFilename_ + "/" + fragmentFilename_);
}

void Shader::setUniform(const std::string name, ivec2 value) {
    GLint uniformLocation = glGetUniformLocation(id_, name.c_str());
    GLint* glvalues = new GLint[2];
    glvalues[0] = value[0];
    glvalues[1] = value[1];
    if (uniformLocation != -1) glUniform2iv(uniformLocation, 1, glm::value_ptr(value));
    //else
        //LogWarn("Unable to set uniform " + name + " in shader " + vertexFilename_ + "/" + fragmentFilename_);
}

void Shader::setUniform(const std::string name, ivec3 value) {
    GLint uniformLocation = glGetUniformLocation(id_, name.c_str());
    GLint* glvalues = new GLint[3];
    glvalues[0] = value[0];
    glvalues[1] = value[1];
    glvalues[2] = value[2];
    if (uniformLocation != -1) glUniform3iv(uniformLocation, 1, glm::value_ptr(value));
    //else
        //LogWarn("Unable to set uniform " + name + " in shader " + vertexFilename_ + "/" + fragmentFilename_);
}

void Shader::setUniform(const std::string name, ivec4 value) {
    GLint uniformLocation = glGetUniformLocation(id_, name.c_str());
    GLint* glvalues = new GLint[4];
    glvalues[0] = value[0];
    glvalues[1] = value[1];
    glvalues[2] = value[2];
    glvalues[3] = value[3];
    if (uniformLocation != -1) glUniform4iv(uniformLocation, 1, glm::value_ptr(value));
    //else
        //LogWarn("Unable to set uniform " + name + " in shader " + vertexFilename_ + "/" + fragmentFilename_);
}

void Shader::setUniform(const std::string name, GLfloat* value, int count) {
    GLint uniformLocation = glGetUniformLocation(id_, name.c_str());
    GLfloat* glvalues = new GLfloat[4];
    if (uniformLocation != -1) glUniform1fv(uniformLocation, count, value);
    //else
        //LogWarn("Unable to set uniform " + name + " in shader " + vertexFilename_ + "/" + fragmentFilename_);
}

void Shader::setUniform(const std::string name, mat4 value) {
    GLint uniformLocation = glGetUniformLocation(id_, name.c_str());
    if (uniformLocation != -1) glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
    //else
        //LogWarn("Unable to set uniform " + name + " in shader " + vertexFilename_ + "/" + fragmentFilename_);
}

} // namespace
