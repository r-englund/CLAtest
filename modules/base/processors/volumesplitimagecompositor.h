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
    virtual Processor* create() const;

    void initialize();
    void deinitialize();

    virtual std::string getClassName() const { return "VolumeSplitImageCompositor"; }
    virtual std::string getCategory() const  { return "Network"; }
    virtual CodeState getCodeState() const   { return CODE_STATE_EXPERIMENTAL; }

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
