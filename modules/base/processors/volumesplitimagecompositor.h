#ifndef IVW_VOLUMESPLITIMAGECOMPOSITOR_H
#define IVW_VOLUMESPLITIMAGECOMPOSITOR_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/volumeport.h>
#include <modules/base/basemoduledefine.h>
#include <modules/opengl/processorgl.h>
#include <modules/opengl/glwrap/shader.h>

namespace inviwo {

class IVW_MODULE_BASE_API VolumeSplitImageCompositor : public ProcessorGL {

public:
    VolumeSplitImageCompositor();
    ~VolumeSplitImageCompositor();
    
    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

protected:
    virtual void process();
    virtual void compile();

private:
    ImageInport inport_;
    ImageOutport outport_;

    int pingPongMod;

    Shader* shader_;
};

}

#endif
