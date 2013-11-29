#ifndef IVW_IMAGECLASSIFY_H
#define IVW_IMAGECLASSIFY_H

#include <modules/basegl/baseglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/properties/transferfunctionproperty.h>
#include <modules/opengl/inviwoopengl.h>
#include <modules/opengl/processorgl.h>
#include <modules/opengl/glwrap/shader.h>

namespace inviwo {

class IVW_MODULE_BASEGL_API ImageClassify : public ProcessorGL {
public:
    ImageClassify();
    ~ImageClassify();

    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

protected:
    virtual void process();

private:
    ImageInport inport_;
    ImageOutport outport_;

    TransferFunctionProperty transferFunction_;

    Shader* shader_;
};

} // namespace

#endif // IVW_IMAGECLASSIFY_H
