#ifndef IVW_BACKGROUND_H
#define IVW_BACKGROUND_H

#include <modules/basegl/baseglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/imageport.h>
#include <modules/opengl/inviwoopengl.h>
#include <modules/opengl/processorgl.h>
#include <modules/opengl/glwrap/shader.h>

namespace inviwo {

class IVW_MODULE_BASEGL_API Background : public ProcessorGL {
public:
    Background();
    ~Background();
    
    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

	virtual void initializeResources();

protected:
    virtual void process();

private:
    ImageInport inport_;
    ImageOutport outport_;

	OptionPropertyInt backgroundStyle_;
	FloatVec4Property color1_;
	FloatVec4Property color2_;

    Shader* shader_;
};

} // namespace

#endif // IVW_BACKGROUND_H
