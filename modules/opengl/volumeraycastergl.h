#ifndef IVW_VOLUMERAYCASTERGL_H
#define IVW_VOLUMERAYCASTERGL_H

#include <modules/opengl/openglmoduledefine.h>
#include "inviwoopengl.h"
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/volumeport.h>
#include <modules/opengl/processorgl.h>
#include <modules/opengl/volumegl.h>
#include <modules/opengl/glwrap/shader.h>
#include <modules/opengl/glwrap/textureunit.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API VolumeRaycasterGL : public ProcessorGL {

public:
    VolumeRaycasterGL();
    VolumeRaycasterGL(std::string programFileName);

    void initialize();
    void deinitialize();

    // overloaded method called when INVALID_RESOURCE property is set
    virtual void initializeResources();

    void bindVolume(const VolumeInport& inport, GLenum volTexUnit);

protected:
    std::string programFileName_;
    Shader* raycastPrg_;

};

} // namespace

#endif // IVW_VOLUMERAYCASTERGL_H
