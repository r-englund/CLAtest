#ifndef IVW_PICKINGCONTAINER_H
#define IVW_PICKINGCONTAINER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/interaction/pickingmanager.h>

namespace inviwo {

class IVW_CORE_API PickingContainer {
public:
    PickingContainer() {
        src_ = NULL;
        selected_ = false;
        currentPickObj_ = NULL;
        prevCoord_ = uvec2(0,0);
    };
    virtual ~PickingContainer() {};

    inline bool isPickableSelected() { return selected_; }

    inline bool performPick(const uvec2& coord) {
        prevCoord_ = coord;
        const ImageRAM* imageRAM = src_->getRepresentation<ImageRAM>();
        vec4 value = imageRAM->getPickingValue(coord);
        vec3 pickedColor = (value.a > 0.f ? value.rgb() : vec3(0.f));
        DataVec3UINT8::type pickedColorUINT8;
        DataVec3UINT8::get()->vec3ToValue(pickedColor*255.f, &pickedColorUINT8);
        currentPickObj_ = PickingManager::instance()->getPickingObjectFromColor(pickedColorUINT8);
        if(currentPickObj_){
            setPickableSelected(true);
            currentPickObj_->setPickingMove(vec2(0.f,0.f));
            currentPickObj_->picked();
            return true;
        }
        else{
            setPickableSelected(false);
            return false;
        }
    }

    inline void movePicked(const uvec2& coord){
        currentPickObj_->setPickingMove(pixelMoveVector(prevCoord_, coord));
        prevCoord_ = coord;
        currentPickObj_->picked();
    }

    inline void setPickableSelected(bool selected){ selected_ = selected; }
    inline void setPickingSource(const Image* src) { src_ = src; }

protected:
    inline vec2 pixelMoveVector(const uvec2& previous, const uvec2& current){
        return vec2(static_cast<float>(current.x-previous.x)/static_cast<float>(src_->getDimension().x), 
            static_cast<int>(current.y-previous.y)/static_cast<float>(src_->getDimension().y));
    }

private:
    const Image* src_;
    PickingObject* currentPickObj_;
    uvec2 prevCoord_;
    bool selected_;
};

} // namespace

#endif // IVW_PICKINGCONTAINER_H