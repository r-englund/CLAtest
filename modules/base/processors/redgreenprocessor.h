#ifndef IVW_REDGREENPROCESSOR_H
#define IVW_REDGREENPROCESSOR_H

#include "inviwo/core/inviwo.h"
#include "inviwo/core/ports/imageport.h"
#include "inviwo/core/ports/volumeport.h"
#include "../../inviwo/modules/opengl/inviwoopengl.h"
#include "../../inviwo/modules/opengl/processorgl.h"
#include "../../inviwo/modules/opengl/glwrap/shader.h"

namespace inviwo {

class RedGreenProcessor : public ProcessorGL {
public:
    RedGreenProcessor();
    ~RedGreenProcessor();
    virtual Processor* create() const;

    void initialize();
    void deinitialize();

    virtual std::string getClassName() const { return "RedGreenProcessor"; }
    virtual std::string getCategory() const  { return "Dummy Processors"; }
    virtual CodeState getCodeState() const   { return CODE_STATE_STABLE; }

protected:
    virtual void process();

private:
    ImagePort outportRed_;
    ImagePort outportGreen_;
};

} // namespace

#endif // VRN_REDGREENPROCESSOR_H
