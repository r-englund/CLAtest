#ifndef IVW_PICKINGCONTAINER_H
#define IVW_PICKINGCONTAINER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {

class IVW_CORE_API PickingContainer {
public:
    PickingContainer() {};
    virtual ~PickingContainer() {};

    inline bool isPickableSelected() { return selected_; }

    inline void checkPickable(const uvec2& coords) {
        const ImageRAM* imageRAM = src_->getRepresentation<ImageRAM>();
        vec4 value = imageRAM->getPickingValue(coords);
        vec3 pickedColor = (value.a > 0.f ? value.rgb() : vec3(0.f));
        LogInfo("Picked Pixel Value : (" << pickedColor.x << "," << pickedColor.y << "," << pickedColor.z << ")");
    }

    inline void setPickingSource(const Image* src) { src_ = src; }

protected:
    inline void setPickableSelected(bool selected){selected_ = selected; }

private:
    const Image* src_;

    bool selected_;
};

} // namespace

#endif // IVW_PICKINGCONTAINER_H