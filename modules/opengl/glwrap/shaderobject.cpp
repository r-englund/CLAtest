#include "shaderobject.h"
#include <stdio.h>
//#include <iostream>
#include <fstream>

#ifdef WIN32
#define OPEN_FILE(a,b,c) fopen_s(&a, b, c);
#else
#define OPEN_FILE(a,b,c) a = fopen(b, c);
#endif

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

std::string ShaderObject::embeddDefines(std::string source) {
    std::ostringstream result;
    for (size_t i=0; i<shaderDefines_.size(); i++) {
        std::pair<std::string, std::string> curDefine = shaderDefines_[i];
        result << "#define" << curDefine.first << " " << curDefine.second << "\n";
    }
    std::string curLine;
    std::istringstream shaderSource(source);
    while (std::getline(shaderSource, curLine))
        result << curLine << "\n";

    return result.str();
}

std::string ShaderObject::embeddIncludes(std::string source) {
    std::ostringstream result;
    std::string curLine;
    std::istringstream shaderSource(source);
    while (std::getline(shaderSource, curLine)) {
        std::string::size_type posInclude = curLine.find("#include");
        std::string::size_type posComment = curLine.find("//");
        if (posInclude!=std::string::npos && (posComment==std::string::npos || posComment>posInclude)) {
            std::string::size_type pathBegin = curLine.find("\"", posInclude+1);
            std::string::size_type pathEnd = curLine.find("\"", pathBegin+1);
            std::string includeFileName(curLine, pathBegin+1, pathEnd-pathBegin-1);

            // TODO: remove absolute paths
            std::ifstream includeFileStream(std::string(IVW_DIR+"modules/opengl/glsl/"+includeFileName).c_str());
            std::stringstream buffer;
            buffer << includeFileStream.rdbuf();
            std::string includeSource = buffer.str();
            
            if (!includeSource.empty())
                result << embeddIncludes(includeSource) << "\n"; 
        }
        else
            result << curLine << "\n";
    }
    return result.str();
}


void ShaderObject::initialize() {
    // TODO: remove absolute paths
    loadSource(IVW_DIR+"modules/opengl/glsl/"+fileName_);
    std::string sourceStr = std::string(source_);
    sourceStr = embeddDefines(sourceStr);
    sourceStr = embeddIncludes(sourceStr);
    source_ = sourceStr.c_str();
    upload();
    compile();
}

void ShaderObject::deinitialize() {}

void ShaderObject::loadSource(std::string fileName) {
    FILE* file;
    char* fileContent = NULL;
    long len;

    if (fileName.length() > 0) {
        OPEN_FILE(file, fileName.c_str(), "rb");
        if (file != NULL) {
            fseek(file, 0L, SEEK_END);
            len = ftell(file);
            fseek(file, 0L, SEEK_SET);
            fileContent = (char*)malloc(sizeof(char)*(len));
            if(fileContent != NULL){
                fread(fileContent, sizeof(char), len, file);
                fileContent[len] = '\0';
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
