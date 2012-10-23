#include "inviwo/core/properties/cameraproperty.h"
#include "inviwo/core/processors/processor.h"

namespace inviwo {

CameraProperty::CameraProperty(std::string identifier, std::string displayName,
                               vec3 lookFrom, vec3 lookTo, vec3 lookUp)
    : CompositeProperty(identifier, displayName),
    lookFrom_("lookFrom", "Look from", lookFrom, vec3(std::numeric_limits<float>::min()), vec3(std::numeric_limits<float>::max()), vec3(0.1f)),
    lookTo_("lookTo", "Look to", lookTo, vec3(std::numeric_limits<float>::min()), vec3(std::numeric_limits<float>::max()), vec3(0.1f)),
    lookUp_("lookUp", "Look up", lookUp, vec3(std::numeric_limits<float>::min()), vec3(std::numeric_limits<float>::max()), vec3(0.1f))
{
    addProperty(lookFrom_);
    addProperty(lookTo_);
    addProperty(lookUp_);
    updateViewMatrix();
}

CameraProperty::~CameraProperty() {}

void CameraProperty::setLookFrom(vec3 lookFrom) {
    lookFrom_.set(lookFrom);
    updateViewMatrix();
}

void CameraProperty::setLookTo(vec3 lookTo) {
    lookTo_.set(lookTo);
    updateViewMatrix();
}

void CameraProperty::setLookUp(vec3 lookUp) {
    lookUp_.set(lookUp);
    updateViewMatrix();
}

void CameraProperty::updateViewMatrix() {
    viewMatrix_ = glm::lookAt( lookTo_.get(), lookFrom_.get(), lookUp_.get());
}

void CameraProperty::invalidate() {
    Processor* owner = dynamic_cast<Processor*>(getOwner());
    if (owner) owner->invalidate();
}

void CameraProperty::serialize(IvwSerializer& s) const {
    Property::serialize(s) ;
    s.serialize("lookFrom", lookFrom_);
    s.serialize("lookTo", lookTo_);
    s.serialize("lookUp", lookUp_);
}

void CameraProperty::deserialize(IvwDeserializer& d) {
    Property::deserialize(d) ;
    d.deserialize("lookFrom", lookFrom_);
    d.deserialize("lookTo", lookTo_);
    d.deserialize("lookUp", lookUp_);
}

} // namespace
