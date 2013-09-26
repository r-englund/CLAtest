#include <inviwo/core/interaction/pickingobject.h>

namespace inviwo {

PickingObject::PickingObject(size_t id, vec3 c) : id_(id), color_(c) {
    onPickedCallback_ = new SingleCallBack();
};

PickingObject::~PickingObject() {
    delete onPickedCallback_;
}

const size_t& PickingObject::getPickingId() const{ 
    return id_; 
}

const vec3& PickingObject::getPickingColor() const{ 
    return color_; 
}

void PickingObject::picked() const{
    onPickedCallback_->invoke();
}

SingleCallBack* PickingObject::getCallbackContainer(){
    return onPickedCallback_;
}

} // namespace
