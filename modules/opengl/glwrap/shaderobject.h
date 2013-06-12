#ifndef IVW_SHADEROBJECT_H
#define IVW_SHADEROBJECT_H

#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <modules/opengl/inviwoopengl.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API ShaderObject {

public:
    ShaderObject(GLenum shaderType, std::string fileName);
    ~ShaderObject();

    GLuint getID() { return id_; }
    std::string getFileName() { return fileName_; }
    std::string getAbsoluteFileName() { return absoluteFileName_; }
    std::vector<std::string> getIncludeFileNames() { return includeFileNames_; }

    bool loadSource(std::string fileName);
    void preprocess();
    void upload();
    bool compile();
    std::string getShaderInfoLog();

    bool build();
    bool rebuild();

    void addShaderDefine(std::string name, std::string value="");
    void removeShaderDefine(std::string name);
    void clearShaderDefines();

    void addOutDeclaration(std::string name);

private:
    bool initialize();
    void deinitialize();

    std::string embeddDefines(std::string source);
    std::string embeddOutDeclarations(std::string source);
    std::string embeddIncludes(std::string source, std::string fileName);

    int getLogLineNumber(const std::string& compileLogLine);
    std::string reformatShaderInfoLog(const std::string compileLog);

    GLenum shaderType_;
    std::string fileName_;
    std::string absoluteFileName_;
    std::vector<std::string> includeFileNames_;

    GLuint id_;
    std::string source_;
    std::string sourceProcessed_;

    std::vector<std::string> outDeclarations_;
    std::vector<std::pair<std::string, std::string> > shaderDefines_;

    std::vector<std::pair<std::string, unsigned int> > lineNumberResolver_;
};

} // namespace

#endif // IVW_SHADEROBJECT_H
