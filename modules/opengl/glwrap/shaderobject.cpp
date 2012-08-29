#include "shaderobject.h"

namespace inviwo {

const std::string ShaderObject::logSource_ = "ShaderObject";

ShaderObject::ShaderObject(GLenum shaderType, std::string fileName) :
    fileName_(fileName),
    shaderType_(shaderType)
{
    id_ = glCreateShader(shaderType);
}

ShaderObject::~ShaderObject() {
    //free(source_);
}

void ShaderObject::initialize() {
    // TODO: remove absolute paths
    loadSource("D:/inviwo/modules/opengl/glsl/"+fileName_);
    upload();
    compile();
}

void ShaderObject::deinitialize() {}

void ShaderObject::loadSource(std::string fileName) {
    FILE* file;
    char* fileContent = NULL;

    int f, count;
    f = _open(fileName.c_str(), _IOREAD);
    count = _lseek(f, 0, SEEK_END);
    _close(f);

    if (fileName.length() > 0) {
        file = fopen(fileName.c_str(), "rt");
        if (file != NULL) {
            if (count > 0) {
                fileContent = (char*)malloc(sizeof(char)*(count+1));
                count = fread(fileContent, sizeof(char), count, file);
                fileContent[count] = '\0';
            }
            fclose(file);
        }
    }
    source_ = fileContent;
}

void ShaderObject::upload() {
    glShaderSource(id_, 1, &source_, 0);
    LGL_ERROR;
}

std::string ShaderObject::getCompileLog() {
    GLint maxLogLength;
    glGetShaderiv(id_, GL_INFO_LOG_LENGTH , &maxLogLength);
    LGL_ERROR;

    if (maxLogLength > 1) {
        GLchar* compileLog = new GLchar[maxLogLength];
        ivwAssert(compileLog!=0, "could not allocate memory for compiler log");
        GLsizei logLength;
        glGetShaderInfoLog(id_, maxLogLength, &logLength, compileLog);
        std::istringstream compileLogStr(compileLog);
        delete[] compileLog;
        return compileLogStr.str();
    } else return "";
}

void ShaderObject::compile() {
    glCompileShader(id_);
    std::string compilerLog = getCompileLog();
    if (!compilerLog.empty())
        LogInfo(compilerLog);
}

} // namespace
