#ifndef IVW_MULTICHANNELRAYCASTER_H
#define IVW_MULTICHANNELRAYCASTER_H

#include <modules/basegl/baseglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/properties/transferfunctionproperty.h>
#include <inviwo/core/properties/simplelightingproperty.h>
#include <inviwo/core/properties/simpleraycastingproperty.h>
#include <inviwo/core/properties/cameraproperty.h>
#include <inviwo/core/properties/compositeproperty.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/ports/volumeport.h>


namespace inviwo {

class Shader;

class IVW_MODULE_BASEGL_API MultichannelRaycaster  : public Processor {
public:
    MultichannelRaycaster();
    virtual ~MultichannelRaycaster();

    InviwoProcessorInfo();

    virtual void initialize();
    virtual void deinitialize();
    virtual void initializeResources();

protected:
    virtual void process();

private:
    Shader* shader_;
    std::string shaderFileName_;

    VolumeInport volumePort_;
    ImageInport entryPort_;
    ImageInport exitPort_;
    ImageOutport outport_;

    CompositeProperty transferFunctions_;

    SimpleRaycastingProperty raycasting_;
    CameraProperty camera_;
    SimpleLightingProperty lighting_;
};

} // namespace

#endif // IVW_MULTICHANNELRAYCASTER_H

