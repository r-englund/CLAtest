#ifndef IVW_PORTGROUP_H
#define IVW_PORTGROUP_H

#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {

class FrameBufferObject;
class Shader;

class IVW_MODULE_OPENGL_API PortGroup {

public:
    PortGroup();

    void initialize();
    void deinitialize();

    void activate();
    void deactivate();

    void addPort(ImageOutport& port);

    void reattachTargets();

    void addShaderDefines(Shader* shader);

private:
    FrameBufferObject* frameBufferObject_;

    std::vector<ImageOutport*>* ports_;
};

} // namespace

#endif // IVW_PORTGROUP_H
