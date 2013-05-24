#ifndef IVW_IMAGEMIXER_H
#define IVW_IMAGEMIXER_H

#include <modules/base/basemoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/imageport.h>
#include <modules/opengl/inviwoopengl.h>
#include <modules/opengl/processorgl.h>
#include <modules/opengl/glwrap/shader.h>

namespace inviwo {

class IVW_MODULE_BASE_API ImageMixer : public ProcessorGL {
public:
    ImageMixer();
    ~ImageMixer();
    
    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

protected:
    virtual void process();

private:
    ImageInport inport0_;
    ImageInport inport1_;
    ImageOutport outport_;
    FloatProperty alpha_;

    Shader* shader_;
};

} // namespace

#endif // IVW_IMAGEMIXER_H
