#ifndef IVW_SHADERMANAGER_H
#define IVW_SHADERMANAGER_H

#include "modules/opengl/openglmoduledefine.h"
#include "inviwo/core/inviwo.h"
#include "modules/opengl/inviwoopengl.h"
#include "modules/opengl/glwrap/shader.h"
#include "inviwo/core/util/fileobserver.h"
#include "inviwo/core/util/singleton.h"

namespace inviwo {

class IVW_MODULE_OPENGL_API ShaderManager : public Singleton<ShaderManager>, public FileObserver {

public:
    ShaderManager();
    Shader* setupShader(std::string fragmentFilename);
    Shader* setupShader(std::string vertexFilename, std::string fragmentFilename);

    virtual void fileChanged(std::string shaderFilename);

private:
    static const std::string logSource_; ///< Source string to be displayed for log messages.
    std::vector<Shader*> shaders_;
};

} // namespace

#endif // IVW_SHADERMANAGER_H
