#ifndef IVW_VOLUMERAYCASTERGL_H
#define IVW_VOLUMERAYCASTERGL_H

#include <modules/opengl/openglmoduledefine.h>
#include "inviwoopengl.h"
#include <inviwo/core/common/inviwo.h>
#include <modules/opengl/processorgl.h>
#include <inviwo/core/ports/volumeport.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API VolumeRaycasterGL : public ProcessorGL {

public:
    VolumeRaycasterGL();
    virtual ~VolumeRaycasterGL();

    void initialize();
    void deinitialize();
};

} // namespace

#endif // IVW_VOLUMERAYCASTERGL_H
