#include <inviwo/core/interaction/pickingobject.h>

namespace inviwo {

PickingObject::PickingObject(size_t id, vec3 c) : id_(id), color_(c) {};

PickingObject::~PickingObject() {}

} // namespace
