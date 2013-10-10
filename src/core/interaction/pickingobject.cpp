#include <inviwo/core/interaction/pickingobject.h>

namespace inviwo {

PickingObject::PickingObject(size_t id, DataVec3UINT8::type c) : id_(id), colorUINT8_(c) {
    onPickedCallback_ = new SingleCallBack();
    color_ = DataVec3UINT8::get()->valueToNormalizedVec3Float(&c);
}

PickingObject::~PickingObject() {
    delete onPickedCallback_;
}

const size_t& PickingObject::getPickingId() const{ 
    return id_; 
}

const vec3& PickingObject::getPickingColor() const{ 
    return color_; 
}

const DataVec3UINT8::type& PickingObject::getPickingColorAsUINT8() const{ 
    return colorUINT8_; 
}

void PickingObject::picked() const{
    onPickedCallback_->invoke();
}

SingleCallBack* PickingObject::getCallbackContainer(){
    return onPickedCallback_;
}

} // namespace
