#ifndef IVW_IMAGEMIXER_H
#define IVW_IMAGEMIXER_H

#include "modules/base/basemoduledefine.h"
#include "inviwo/core/inviwo.h"
#include "inviwo/core/ports/imageport.h"
#include "modules/opengl/inviwoopengl.h"
#include "modules/opengl/processorgl.h"
#include "modules/opengl/glwrap/shader.h"

namespace inviwo {

class IVW_MODULE_BASE_API ImageMixer : public ProcessorGL {
public:
    ImageMixer();
    ~ImageMixer();
    virtual Processor* create() const;

    void initialize();
    void deinitialize();

    virtual std::string getClassName() const { return "ImageMixer"; }
    virtual std::string getCategory() const  { return "Compositer"; }
    virtual CodeState getCodeState() const   { return CODE_STATE_EXPERIMENTAL; }

protected:
    virtual void process();

private:
    ImagePort inport0_;
    ImagePort inport1_;
    ImagePort outport_;
    FloatProperty alpha_;

    Shader* shader_;
};

} // namespace

#endif // IVW_IMAGEMIXER_H
