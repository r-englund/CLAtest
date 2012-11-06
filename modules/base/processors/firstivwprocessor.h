#ifndef IVW_FIRSTIVWPROCESSOR_H
#define IVW_FIRSTIVWPROCESSOR_H

#include "inviwo/core/inviwo.h"
#include "inviwo/core/ports/imageport.h"
#include "modules/opengl/inviwoopengl.h"
#include "modules/opengl/processorgl.h"
#include "modules/opengl/glwrap/shader.h"

namespace inviwo {

class FirstIvwProcessor : public ProcessorGL {
public:
    FirstIvwProcessor();
    ~FirstIvwProcessor();
    virtual Processor* create() const;

    void initialize();
    void deinitialize();

    virtual std::string getClassName() const { return "FirstIVWProcessor"; }
    virtual std::string getCategory() const  { return "Dummy Processors"; }
    virtual CodeState getCodeState() const   { return CODE_STATE_STABLE; }

protected:
    virtual void process();

private:
    ImagePort outport_;
    FloatVec3Property color_;
};

} // namespace

#endif // VRN_FIRSTIVWPROCESSOR_H
