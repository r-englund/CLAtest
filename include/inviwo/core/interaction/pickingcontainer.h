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
        const ImageRAM* colorImage = src_->getRepresentation<ImageRAM>();
        glm::vec4 value = colorImage->getValueAsVec4Float(coords);
        LogInfo("Picked Pixel Value : (" << value.x << "," << value.y << "," << value.z << ")");
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