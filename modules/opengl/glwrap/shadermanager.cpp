#include "shadermanager.h"
#include "inviwo/core/inviwoapplication.h"

namespace inviwo {

const std::string ShaderManager::logSource_ = "ShaderManager";

ShaderManager::ShaderManager() {
    InviwoApplication::getRef().registerFileObserver(this);
}

Shader* ShaderManager::setupShader(std::string fragmentFilename) {
    Shader* shader = new Shader(fragmentFilename);
    shaders_.push_back(shader);
    // TODO: remove absolute path
    startFileObservation(IVW_DIR+"modules/opengl/glsl/"+fragmentFilename);
    return shader;
}

Shader* ShaderManager::setupShader(std::string vertexFilename, std::string fragmentFilename) {
    Shader* shader = new Shader(vertexFilename, fragmentFilename);
    shaders_.push_back(shader);
    // TODO: remove absolute paths
    startFileObservation(IVW_DIR+"modules/opengl/glsl/"+vertexFilename);
    startFileObservation(IVW_DIR+"modules/opengl/glsl/"+fragmentFilename);
    return shader;
}

void ShaderManager::fileChanged(std::string shaderFilename) {
    LogInfo("Change detected. Reloading " + shaderFilename);
    bool successfulReload = true;
    for (size_t i=0; i<shaders_.size(); i++) {
        bool relink = false;
        std::cout << shaders_[i]->getVertexShaderObject()->getFileName() << std::endl;
        if (shaders_[i]->getVertexShaderObject()->getFileName()==shaderFilename) {
            bool initialized = shaders_[i]->getVertexShaderObject()->initialize();
            if (!initialized) successfulReload = false;
            relink = true;
        }
        std::cout << shaders_[i]->getFragmentShaderObject()->getFileName() << std::endl;
        if (shaders_[i]->getFragmentShaderObject()->getFileName()==shaderFilename) {
            bool initialized = shaders_[i]->getFragmentShaderObject()->initialize();
            if (!initialized) successfulReload = false;
            relink = true;
        }
        if (relink) shaders_[i]->link();
    }
    if (successfulReload) {
        InviwoApplication::getRef().playSound(InviwoApplication::IVW_OK);
        // TODO: only invalidate affected processors
        std::vector<Processor*> processors = InviwoApplication::getRef().getProcessorNetwork()->getProcessors();
        for (size_t i=0;i<processors.size();i++)
            processors[i]->invalidate();
    } else InviwoApplication::getRef().playSound(InviwoApplication::IVW_ERROR);
}

} // namespace
