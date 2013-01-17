#ifndef IVW_SHADERMANAGER_H
#define IVW_SHADERMANAGER_H

#include "modules/opengl/openglmoduledefine.h"
#include "inviwo/core/inviwo.h"
#include "modules/opengl/inviwoopengl.h"
#include "inviwo/core/util/singleton.h"

namespace inviwo {

class IVW_MODULE_OPENGL_API ShaderManager : public Singleton<ShaderManager> {

public:
    ShaderManager();
    void loadShader();

private:
    static const std::string logSource_; ///< Source string to be displayed for log messages.
};

} // namespace

#endif // IVW_SHADERMANAGER_H
