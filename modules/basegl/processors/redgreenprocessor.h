#ifndef IVW_REDGREENPROCESSOR_H
#define IVW_REDGREENPROCESSOR_H

#include <modules/basegl/baseglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/ports/volumeport.h>
#include <modules/opengl/inviwoopengl.h>
#include <modules/opengl/processorgl.h>
#include <modules/opengl/glwrap/shader.h>

namespace inviwo {

class IVW_MODULE_BASEGL_API RedGreenProcessor : public ProcessorGL {
public:
    RedGreenProcessor();
    ~RedGreenProcessor();
    
    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

protected:
    virtual void process();

private:
    ImageOutport outportRed_;
    ImageOutport outportGreen_;
};

} // namespace

#endif // VRN_REDGREENPROCESSOR_H
