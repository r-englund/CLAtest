#include "shaderobject.h"
#include <stdio.h>
#include <fstream>

#include <inviwo/core/util/filedirectory.h>
#include <modules/opengl/glwrap/shadermanager.h>


namespace inviwo {

ShaderObject::ShaderObject(GLenum shaderType, std::string fileName, bool compileShader) :
    shaderType_(shaderType),
    fileName_(fileName)
{
    initialize(compileShader);
}

ShaderObject::~ShaderObject() {
    glDeleteShader(id_);
}

bool ShaderObject::initialize(bool compileShader) {
    id_ = glCreateShader(shaderType_);
    LGL_ERROR;
    loadSource(fileName_);
    preprocess();
    upload();
    bool result = true;
    if (compileShader) result = compile();
    return result;
}

bool ShaderObject::build() {
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
    lineNumberResolver_.clear();
    includeFileNames_.clear();
    std::string shaderHeader = embeddDefines(source_);
    shaderHeader += embeddOutDeclarations(source_);
    sourceProcessed_ = shaderHeader + embeddIncludes(source_, fileName_);
}

std::string ShaderObject::embeddDefines(std::string source) {
    std::ostringstream result;
    for (size_t i=0; i<shaderDefines_.size(); i++) {
        std::pair<std::string, std::string> curDefine = shaderDefines_[i];
        result << "#define " << curDefine.first << " " << curDefine.second << "\n";
        lineNumberResolver_.push_back(std::pair<std::string, unsigned int>("Define", 0));
    }
    std::string curLine;
    std::string globalGLSLHeader = ShaderManager::getRef().getGlobalGLSLHeader();
    std::istringstream globalGLSLHeaderStream(globalGLSLHeader);
    while (std::getline(globalGLSLHeaderStream, curLine))
        lineNumberResolver_.push_back(std::pair<std::string, unsigned int>("GlobalGLSLSHeader", 0));
    return globalGLSLHeader + result.str();
}

std::string ShaderObject::embeddOutDeclarations(std::string source) {
    std::ostringstream result;
    for (size_t i=0; i<outDeclarations_.size(); i++) {
        std::string curDeclaration = outDeclarations_[i];
        result << "out vec4 " << curDeclaration << ";\n";
        lineNumberResolver_.push_back(std::pair<std::string, unsigned int>("Out Declaration", 0));
    }
    return result.str();
}

std::string ShaderObject::embeddIncludes(std::string source, std::string fileName) {
    std::ostringstream result;
    std::string curLine;
    std::istringstream shaderSource(source);
    int localLineNumber = 1;
    while (std::getline(shaderSource, curLine)) {
        std::string::size_type posInclude = curLine.find("#include");
        std::string::size_type posComment = curLine.find("//");
        if (posInclude!=std::string::npos && (posComment==std::string::npos || posComment>posInclude)) {
            std::string::size_type pathBegin = curLine.find("\"", posInclude+1);
            std::string::size_type pathEnd = curLine.find("\"", pathBegin+1);
            std::string includeFileName(curLine, pathBegin+1, pathEnd-pathBegin-1);

            bool includeFileFound = false;
            std::vector<std::string> shaderSearchPaths = ShaderManager::getRef().getShaderSearchPaths();
            for (size_t i=0; i<shaderSearchPaths.size(); i++) {
                if (URLParser::fileExists(shaderSearchPaths[i]+"/"+includeFileName)) {
                    includeFileName = shaderSearchPaths[i]+"/"+includeFileName;
                    includeFileNames_.push_back(includeFileName);
                    std::ifstream includeFileStream(includeFileName.c_str());
                    std::stringstream buffer;
                    buffer << includeFileStream.rdbuf();
                    std::string includeSource = buffer.str();            
                    if (!includeSource.empty())
                        result << embeddIncludes(includeSource, includeFileName);// << "\n";
                    includeFileFound = true;
                    break;
                }
            }
            if (!includeFileFound)
                LogWarn("Include file " << includeFileName << " not found in shader search paths.");
        } else {
            result << curLine << "\n";
            lineNumberResolver_.push_back(std::pair<std::string, unsigned int>(fileName, localLineNumber));
        }
        localLineNumber++;
    } 
    return result.str();
}

bool ShaderObject::loadSource(std::string fileName) {
    source_ = "";
    if (fileName.length() > 0) {
        std::vector<std::string> shaderSearchPaths = ShaderManager::getRef().getShaderSearchPaths();
        for (size_t i=0; i<shaderSearchPaths.size(); i++) {
            if (URLParser::fileExists(shaderSearchPaths[i]+"/"+fileName)) {
                absoluteFileName_ = shaderSearchPaths[i]+"/"+fileName;
                break;
            }
        }
        std::ifstream fileStream(absoluteFileName_.c_str());
        std::stringstream buffer;
        buffer << fileStream.rdbuf();
        source_ = buffer.str();
        return true;
    } else return false;
}

void ShaderObject::upload() {
    const char* source = sourceProcessed_.c_str();
    glShaderSource(id_, 1, &source, 0);
    LGL_ERROR;
}

std::string ShaderObject::getShaderInfoLog() {
    GLint maxLogLength;
    glGetShaderiv(id_, GL_INFO_LOG_LENGTH , &maxLogLength);
    LGL_ERROR;
    if (maxLogLength > 1) {
        GLchar* shaderInfoLog = new GLchar[maxLogLength];
        ivwAssert(shaderInfoLog!=0, "could not allocate memory for compiler log");
        GLsizei logLength;
        glGetShaderInfoLog(id_, maxLogLength, &logLength, shaderInfoLog);
        std::istringstream shaderInfoLogStr(shaderInfoLog);
        delete[] shaderInfoLog;
        return shaderInfoLogStr.str();
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

std::string ShaderObject::reformatShaderInfoLog(const std::string shaderInfoLog) {
    std::ostringstream result;
    std::string curLine;
    std::istringstream origShaderInfoLog(shaderInfoLog);
    while (std::getline(origShaderInfoLog, curLine)) {
        unsigned int origLineNumber = getLogLineNumber(curLine);
        unsigned int lineNumber = lineNumberResolver_[origLineNumber-1].second;
        std::string fileName = lineNumberResolver_[origLineNumber-1].first;
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
        std::string compilerLog = getShaderInfoLog();
        compilerLog = reformatShaderInfoLog(compilerLog);
        LogError(compilerLog);
        return false;
    }
    return true;
}

void ShaderObject::addShaderDefine(std::string name, std::string value) {
    removeShaderDefine(name);
    shaderDefines_.push_back(std::pair<std::string,std::string>(name, value));
}

void ShaderObject::removeShaderDefine(std::string name) {
    for (size_t i=0; i<shaderDefines_.size(); i++)
        if (shaderDefines_[i].first == name) {
            shaderDefines_.erase(shaderDefines_.begin()+i);
            i = shaderDefines_.size();
        }
}

void ShaderObject::clearShaderDefines() {
    shaderDefines_.clear();
}

void ShaderObject::addOutDeclaration(std::string name) {
    bool outExists = false;
    for (size_t i=0; i<outDeclarations_.size(); i++)
        if (outDeclarations_[i] == name)
            outExists = true;
    if (!outExists)
        outDeclarations_.push_back(name);
}

} // namespace
