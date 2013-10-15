#ifndef IVW_PICKINGOBJECT_H
#define IVW_PICKINGOBJECT_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/util/callback.h>

namespace inviwo {

/** \class PickingObject 
*/
class IVW_CORE_API PickingObject {

public:
    PickingObject(size_t, DataVec3UINT8::type);

    virtual ~PickingObject();

    const size_t& getPickingId() const;
    const vec3& getPickingColor() const;
    const DataVec3UINT8::type& getPickingColorAsUINT8() const;
    const vec2& getPickingMove() const;

    void picked() const;

    void setPickingMove(vec2);

    SingleCallBack* getCallbackContainer();

private:
    size_t id_;
    DataVec3UINT8::type colorUINT8_;
    vec3 color_;
    vec2 move_;
    SingleCallBack* onPickedCallback_;
};

} // namespace

#endif // IVW_PICKINGOBJECT_H