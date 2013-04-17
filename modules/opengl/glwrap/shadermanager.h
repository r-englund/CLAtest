#ifndef IVW_SHADERMANAGER_H
#define IVW_SHADERMANAGER_H

#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <modules/opengl/inviwoopengl.h>
#include <modules/opengl/openglinfo.h>
#include <modules/opengl/glwrap/shader.h>
#include <inviwo/core/util/fileobserver.h>
#include <inviwo/core/util/singleton.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API ShaderManager : public Singleton<ShaderManager>, public FileObserver {

public:
    ShaderManager();

    void registerShader(Shader* shader);
    void unregisterShader(Shader* shader);

    virtual void fileChanged(std::string shaderFilename);

    std::string getGlobalGLSLHeader();

private:
    std::vector<Shader*> shaders_;
    OpenGLInfo* openGLInfoRef_;
};

} // namespace

#endif // IVW_SHADERMANAGER_H
