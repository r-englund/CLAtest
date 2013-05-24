#ifndef IVW_VOLUMEPROCESSORGL_H
#define IVW_VOLUMEPROCESSORGL_H

#include <modules/opengl/openglmoduledefine.h>
#include "inviwoopengl.h"
#include <inviwo/core/common/inviwo.h>
#include <modules/opengl//processorgl.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API VolumeProcessorGL : public ProcessorGL {

public:
    VolumeProcessorGL();
    virtual ~VolumeProcessorGL();
    virtual Processor* create() const;

    void initialize();
    void deinitialize();
};

} // namespace

#endif // IVW_VOLUMEPROCESSORGL_H
