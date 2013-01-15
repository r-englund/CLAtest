#ifndef IVW_SHADEROBJECT_H
#define IVW_SHADEROBJECT_H

#include "inviwo/core/inviwo.h"
#include "modules/opengl/inviwoopengl.h"

namespace inviwo {

class ShaderObject {

public:
    ShaderObject(GLenum shaderType, std::string fileName);
    ~ShaderObject();

    GLuint getID() { return id_; }

    void initialize();
    void deinitialize();

    void loadSource(std::string fileName);
    void preprocess();
    void upload();
    void compile();
    std::string getCompileLog();

private:
    std::string embeddDefines(std::string source);
    std::string embeddIncludes(std::string source);

    GLenum shaderType_;
    std::string fileName_;

    GLuint id_;
    const char* source_;

    std::vector<std::pair<std::string, std::string> > shaderDefines_;

    static const std::string logSource_; ///< Source string to be displayed for log messages.
};

} // namespace

#endif // IVW_SHADEROBJECT_H
