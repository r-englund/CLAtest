#include "shaderobject.h"
#include <stdio.h>
#include <fstream>

#include <modules/opengl/glwrap/shadermanager.h>

#ifdef WIN32
#define OPEN_FILE(a,b,c) fopen_s(&a, b, c);
#else
#define OPEN_FILE(a,b,c) a = fopen(b, c);
#endif

namespace inviwo {

ShaderObject::ShaderObject(GLenum shaderType, std::string fileName) :
    shaderType_(shaderType),
    fileName_(fileName)
    
{
    initialize();
}

ShaderObject::~ShaderObject() {
    glDeleteShader(id_);
}

bool ShaderObject::initialize() {
    id_ = glCreateShader(shaderType_);
    LGL_ERROR;
    loadSource(fileName_);
    preprocess();
    upload();
    return compile();
}

bool ShaderObject::rebuild() {
    if (loadSource(fileName_)) {
        preprocess();
        upload();
        return compile();
    } else return false;
}

void ShaderObject::preprocess() {
    sourceProcessed_ = embeddDefines(source_);
    includeFileNames_.clear();
    lineNumberResolver_.clear();
    sourceProcessed_ = embeddIncludes(sourceProcessed_, fileName_);
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

    return ShaderManager::getRef().getGlobalGLSLHeader() + result.str();
}

std::string ShaderObject::embeddIncludes(std::string source, std::string fileName) {
    std::ostringstream result;
    std::string curLine;
    std::istringstream shaderSource(source);
    unsigned int localLineNumber = 0;
    while (std::getline(shaderSource, curLine)) {
        std::string::size_type posInclude = curLine.find("#include");
        std::string::size_type posComment = curLine.find("//");
        if (posInclude!=std::string::npos && (posComment==std::string::npos || posComment>posInclude)) {
            std::string::size_type pathBegin = curLine.find("\"", posInclude+1);
            std::string::size_type pathEnd = curLine.find("\"", pathBegin+1);
            std::string includeFileName(curLine, pathBegin+1, pathEnd-pathBegin-1);

            // TODO: remove absolute path
            includeFileName = IVW_DIR+"modules/opengl/glsl/"+includeFileName;
            includeFileNames_.push_back(includeFileName);
            std::ifstream includeFileStream(includeFileName.c_str());
            std::stringstream buffer;
            buffer << includeFileStream.rdbuf();
            std::string includeSource = buffer.str();            
            if (!includeSource.empty())
                result << embeddIncludes(includeSource, includeFileName) << "\n";
        }
        else
            result << curLine << "\n";
        lineNumberResolver_.push_back(std::pair<std::string, unsigned int>(fileName, localLineNumber));
        localLineNumber++;
    }
    return result.str();
}

bool ShaderObject::loadSource(std::string fileName) {
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
        } else return false;
    } else return false;
    source_ = std::string(fileContent);
    return true;
}

void ShaderObject::upload() {
    const char* source = sourceProcessed_.c_str();
    glShaderSource(id_, 1, &source, 0);
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

int ShaderObject::getLogLineNumber(const std::string& compileLogLine) {
    // TODO: adapt to ATI compile log syntax
    int result = 0;
    std::istringstream input(compileLogLine);
    int num;
    if (input>>num) {
        char c;
        if (input>>c && c=='(') {
            if (input>>result) {
                return result;
            }
        }
    }
    return result;
}

std::string ShaderObject::reformatCompileLog(const std::string compileLog) {
    std::ostringstream result;
    std::string curLine;
    std::istringstream origCompileLog(compileLog);
    while (std::getline(origCompileLog, curLine)) {
        unsigned int origLineNumber = getLogLineNumber(curLine);
        unsigned int lineNumber = lineNumberResolver_[origLineNumber].second;
        std::string fileName = lineNumberResolver_[origLineNumber].first;
        // TODO: adapt substr call to ATI compile log syntax
        result << "\n" << fileName << " (" << lineNumber << "): " << curLine.substr(curLine.find(":")+1);
    }
    return result.str();
}

bool ShaderObject::compile() {
    glCompileShader(id_);
    GLint compiledOk = 0;
    glGetShaderiv(id_, GL_COMPILE_STATUS, &compiledOk);
    if (!compiledOk) {
        std::string compilerLog = getCompileLog();
        compilerLog = reformatCompileLog(compilerLog);
        LogError(compilerLog);
        return false;
    }
    return true;
}

} // namespace
