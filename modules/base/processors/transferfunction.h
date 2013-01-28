#ifndef IVW_TRANSFERFUNCTION_H
#define IVW_TRANSFERFUNCTION_H

#include <modules/base/basemoduledefine.h>
#include <inviwo/core/inviwo.h>
#include <inviwo/core/ports/imageport.h>
#include <modules/opengl/inviwoopengl.h>
#include <modules/opengl/processorgl.h>
#include <modules/opengl/glwrap/shader.h>

namespace inviwo {

class IVW_MODULE_BASE_API TransferFunction : public ProcessorGL {
public:
    TransferFunction();
    ~TransferFunction();
    virtual Processor* create() const;

    void initialize();
    void deinitialize();

    virtual std::string getClassName() const { return "TransferFunction"; }
    virtual std::string getCategory() const  { return "Compositer"; }
    virtual CodeState getCodeState() const   { return CODE_STATE_EXPERIMENTAL; }

protected:
    virtual void process();

private:
	//BoolProperty bool_;
	TransferProperty trans_;
	//BoolProperty bool2_;

    Shader* shader_;
};

} // namespace

#endif // IVW_TRANSFERFUNCTION_H
