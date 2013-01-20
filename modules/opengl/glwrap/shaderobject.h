#ifndef IVW_SHADEROBJECT_H
#define IVW_SHADEROBJECT_H

#include "modules/opengl/openglmoduledefine.h"
#include "inviwo/core/inviwo.h"
#include "modules/opengl/inviwoopengl.h"

namespace inviwo {

class IVW_MODULE_OPENGL_API ShaderObject {

public:
    ShaderObject(GLenum shaderType, std::string fileName);
    ~ShaderObject();

    GLuint getID() { return id_; }
    std::string getFileName() { return fileName_; }

    bool initialize();
    void deinitialize();

    void loadSource(std::string fileName);
    void preprocess();
    void upload();
    bool compile();
    std::string getCompileLog();

private:
    std::string embeddDefines(std::string source);
    std::string embeddIncludes(std::string source, std::string fileName);

    int getLogLineNumber(const std::string& compileLogLine);
    std::string reformatCompileLog(const std::string compileLog);

    GLenum shaderType_;
    std::string fileName_;

    GLuint id_;
    const char* source_;

    std::vector<std::pair<std::string, std::string> > shaderDefines_;
    std::vector<std::pair<std::string, unsigned int> > lineNumberResolver_;

    static const std::string logSource_; ///< Source string to be displayed for log messages.
};

} // namespace

#endif // IVW_SHADEROBJECT_H
