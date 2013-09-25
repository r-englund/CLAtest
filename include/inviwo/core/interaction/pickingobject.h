#ifndef IVW_PICKINGOBJECT_H
#define IVW_PICKINGOBJECT_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {

/** \class PickingObject 
*/
class IVW_CORE_API PickingObject {

public:
    PickingObject(size_t, vec3);

    virtual ~PickingObject();

    const size_t& getPickingId() const{ return id_; }
    const vec3& getPickingColor() const{ return color_; }

private:
    size_t id_;
    vec3 color_;

};

} // namespace

#endif // IVW_PICKINGOBJECT_H