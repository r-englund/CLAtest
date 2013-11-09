#ifndef IVW_CANVASPROCESSORGL_H
#define IVW_CANVASPROCESSORGL_H

#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/util/canvas.h>
#include <inviwo/core/processors/canvasprocessor.h>

#include "processorgl.h"

namespace inviwo {

class IVW_MODULE_OPENGL_API CanvasProcessorGL : public CanvasProcessor {
public:
    CanvasProcessorGL();
    
    InviwoProcessorInfo();

    virtual void initialize();
    virtual void deinitialize();

protected:
    virtual void process();

};

} // namespace

#endif // IVW_CANVASPROCESSORGL_H
