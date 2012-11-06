#ifndef IVW_IMAGESOURCE_H
#define IVW_IMAGESOURCE_H

#include "inviwo/core/inviwo.h"
#include "inviwo/core/processors/processor.h"
#include "inviwo/core/ports/imageport.h"
#include "modules/opengl/inviwoopengl.h"
#include "modules/opengl/processorgl.h"
#include "modules/opengl/glwrap/shader.h"

namespace inviwo {

class ImageSource : public ProcessorGL {
public:
    ImageSource();
    ~ImageSource();
    virtual Processor* create() const;

    virtual void initialize();
    virtual void deinitialize();

    virtual std::string getClassName() const { return "ImageSource"; }
    virtual std::string getCategory() const  { return "Data Source Processor"; }
    virtual CodeState getCodeState() const   { return CODE_STATE_STABLE; }

protected:
    virtual void process();

private:
    ImagePort outport_;
    FileProperty imageFileName_;

    Shader* shader_;
};

} // namespace

#endif // IVW_IMAGESOURCE_H
