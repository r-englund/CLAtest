#ifndef IVW_CANVASPROCESSORGL_H
#define IVW_CANVASPROCESSORGL_H

#include "inviwo/core/inviwo.h"
#include "inviwo/core/ports/imageport.h"
#include "inviwo/core/util/canvas.h"
#include "inviwo/core/processors/canvasprocessor.h"

#include "processorgl.h"
#include "glwrap/shader.h"

namespace inviwo {

    class CanvasProcessorGL : public CanvasProcessor {
    public:
        CanvasProcessorGL();

        virtual void initialize();
        virtual void deinitialize();

        virtual std::string getClassName() const { return "CanvasProcessorGL"; }
        virtual std::string getCategory() const  { return "Canvas Processor"; }
        virtual CodeState getCodeState() const   { return CODE_STATE_STABLE; }

    protected:
        virtual void process();

    private:
        Shader* shader_;

        void renderImagePlaneQuad() const;
    };

} // namespace

#endif // IVW_CANVASPROCESSORGL_H
