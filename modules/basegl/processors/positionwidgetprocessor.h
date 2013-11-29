#ifndef IVW_POSITIONWIDGETPROCESSOR_H
#define IVW_POSITIONWIDGETPROCESSOR_H

#include <modules/basegl/baseglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <modules/opengl/inviwoopengl.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/ports/geometryport.h>
#include <modules/opengl/image/compositeprocessorgl.h>
#include <inviwo/core/interaction/pickingobject.h>
#include <inviwo/core/properties/vectorproperties.h>

namespace inviwo {

class IVW_MODULE_BASEGL_API PositionWidgetProcessor : public CompositeProcessorGL {
public:
    PositionWidgetProcessor();
    ~PositionWidgetProcessor();

    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

    bool isReady() const { return geometryInport_.isReady(); }

    void updateWidgetPositionFromPicking();

protected:
    virtual void process();

private:
    GeometryInport geometryInport_;
    ImageInport imageInport_;
    ImageOutport outport_;

    FloatVec3Property position_;

    CameraProperty camera_;

    const PickingObject* widgetPickingObject_;

    Shader* program_;
};

} // namespace

#endif // IVW_POSITIONWIDGETPROCESSOR_H
