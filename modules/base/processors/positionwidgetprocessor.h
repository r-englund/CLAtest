#ifndef IVW_POSITIONWIDGETPROCESSOR_H
#define IVW_POSITIONWIDGETPROCESSOR_H

#include <modules/base/basemoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <modules/opengl/inviwoopengl.h>
#include <inviwo/core/ports/imageport.h>
#include <modules/opengl/image/compositeprocessorgl.h>
#include <inviwo/core/interaction/pickingobject.h>
#include <inviwo/core/properties/optionproperties.h>
#include <inviwo/core/properties/vectorproperties.h>

namespace inviwo {

class IVW_MODULE_BASE_API PositionWidgetProcessor : public CompositeProcessorGL {
public:
    PositionWidgetProcessor();
    ~PositionWidgetProcessor();

    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

    bool isReady() const { return true; }

    void updateWidgetPositionFromPicking();

protected:
    virtual void process();

private:
    ImageInport inport_;
    ImageOutport outport_;

    OptionPropertyString widgetType_;
    FloatVec3Property position_;

    CameraProperty camera_;

    const PickingObject* widgetPickingObject_;

    Geometry* widget_;

    Shader* program_;
};

} // namespace

#endif // IVW_POSITIONWIDGETPROCESSOR_H
