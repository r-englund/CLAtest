#ifndef IVW_IMAGEGRAYSCALE_H
#define IVW_IMAGEGRAYSCALE_H

#include <modules/base/basemoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/imageport.h>
#include <modules/opengl/inviwoopengl.h>
#include <modules/opengl/processorgl.h>
#include <modules/opengl/glwrap/shader.h>

namespace inviwo {

class IVW_MODULE_BASE_API ImageGrayscale : public ProcessorGL {
public:
    ImageGrayscale();
    ~ImageGrayscale();
    virtual Processor* create() const;

    void initialize();
    void deinitialize();

    virtual std::string getClassName() const { return "ImageGrayscale"; }
    virtual std::string getCategory() const  { return "Image Converter"; }
    virtual CodeState getCodeState() const   { return CODE_STATE_EXPERIMENTAL; }

protected:
    virtual void process();

private:
    ImageInport inport0_;
    ImageOutport outport_;

    Shader* shader_;
};

} // namespace

#endif // IVW_IMAGEGRAYSCALE_H
