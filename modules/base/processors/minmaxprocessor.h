#ifndef IVW_MINMAXPROCESSOR_H
#define IVW_MINMAXPROCESSOR_H

#include <modules/base/basemoduledefine.h>
#include <inviwo/core/inviwo.h>

#include <modules/opengl/glwrap/shader.h>

#include <modules/opengl/processorgl.h>

#include <inviwo/core/datastructures/imageram.h>
#include <inviwo/core/datastructures/imagedisk.h>
namespace inviwo {

class IVW_MODULE_BASE_API MinMaxProcessor : public ProcessorGL {
public:
    MinMaxProcessor();
    ~MinMaxProcessor();
    virtual Processor* create() const;

    void initialize();
    void deinitialize();

    virtual std::string getClassName() const { return "MinMaxProcessor"; }
    virtual std::string getCategory() const  { return "Dummy Processors"; }
    virtual CodeState getCodeState() const   { return CODE_STATE_STABLE; }

protected:
    virtual void process();

private:
    
    uvec2 calculateMinMaxValues(uint8_t *data, int size) const;

	ImagePort inport0_;
    ImagePort outport_;
    Shader* shader_;
};

} // namespace

#endif // IVW_MINMAXPROCESSOR_H
