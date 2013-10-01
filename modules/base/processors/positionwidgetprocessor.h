#ifndef IVW_POSITIONWIDGETPROCESSOR_H
#define IVW_POSITIONWIDGETPROCESSOR_H

#include <modules/base/basemoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <modules/opengl/inviwoopengl.h>
#include <inviwo/core/ports/imageport.h>
#include <modules/opengl/processorgl.h>
#include <inviwo/core/interaction/pickingobject.h>
#include <inviwo/core/properties/optionproperties.h>
#include <inviwo/core/properties/vectorproperties.h>

namespace inviwo {

class IVW_MODULE_BASE_API PositionWidgetProcessor : public ProcessorGL {
public:
    PositionWidgetProcessor();
    ~PositionWidgetProcessor();

    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

    void updateWidgetPositionFromPicking();

protected:
    virtual void process();

private:
    ImageInport inport_;
    ImageOutport outport_;

    OptionPropertyString widgetType_;

    FloatVec3Property position_;

    const PickingObject* widgetPickingObject_;

    Geometry* widget_;
};

} // namespace

#endif // IVW_POSITIONWIDGETPROCESSOR_H
