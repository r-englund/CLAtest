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
    Processor* create() const;

    virtual void initialize();
    virtual void deinitialize();

    virtual std::string getClassName() const { return "CanvasProcessorGL"; }
    virtual std::string getCategory() const  { return "Canvas Processor"; }
    virtual CodeState getCodeState() const   { return CODE_STATE_STABLE; }

protected:
    virtual void process();

    IntVec2Property dimensions_;

private:
    void resizeCanvas();
};

} // namespace

#endif // IVW_CANVASPROCESSORGL_H
