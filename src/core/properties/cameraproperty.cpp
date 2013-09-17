#include <inviwo/core/properties/cameraproperty.h>
#include <inviwo/core/processors/processor.h>

namespace inviwo {

CameraProperty::CameraProperty(std::string identifier, std::string displayName, 
                               vec3 eye, vec3 center, vec3 lookUp,
                               PropertyOwner::InvalidationLevel invalidationLevel, PropertySemantics::Type semantics)
    : CompositeProperty(identifier, displayName, invalidationLevel, semantics),
    lookFrom_("lookFrom", "Look from", eye, -vec3(10.0f), vec3(10.0f), vec3(0.1f), invalidationLevel),
    lookTo_("lookTo", "Look to", center, -vec3(10.0f), vec3(10.0f), vec3(0.1f), invalidationLevel),
    lookUp_("lookUp", "Look up", lookUp, -vec3(10.0f), vec3(10.0f), vec3(0.1f), invalidationLevel),
    fovy_("fov", "FOV", 60.0f, 30.0f, 360.0f, 0.1f, invalidationLevel),
    aspectRatio_("aspectRatio", "Aspect Ratio", 256.0f/256.0f, 0.0f, 1.0f, 0.1f, invalidationLevel),
    nearPlane_("near", "Near Plane", 0.0001f, 0.0f, 1000.0f, 0.1f, invalidationLevel),
    farPlane_("far", "Far Plane", 100.0f, 0.0f, 1000.0f, 0.1f, invalidationLevel)

{
    lookFrom_.onChange(this, &CameraProperty::updateViewMatrix);
    lookTo_.onChange(this, &CameraProperty::updateViewMatrix);
    lookUp_.onChange(this, &CameraProperty::updateViewMatrix);
    addProperty(lookFrom_);
    addProperty(lookTo_);
    addProperty(lookUp_);

    fovy_.onChange(this, &CameraProperty::updateProjectionMatrix);
    aspectRatio_.onChange(this, &CameraProperty::updateProjectionMatrix);
    nearPlane_.onChange(this, &CameraProperty::updateProjectionMatrix);
    farPlane_.onChange(this, &CameraProperty::updateProjectionMatrix);
    addProperty(fovy_);
    addProperty(aspectRatio_);
    addProperty(nearPlane_);
    addProperty(farPlane_);

    updateViewMatrix();
    updateProjectionMatrix();
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


void CameraProperty::setProjectionMatrix(float fovy, float aspect, float nearPlane, float farPlane) {
    fovy_.set(fovy);
    aspectRatio_.set(aspect);
    farPlane_.set(farPlane);
    nearPlane_.set(nearPlane);
    updateProjectionMatrix();
}

void CameraProperty::updateProjectionMatrix() {
    projectionMatrix_ = glm::perspective(fovy_.get(), aspectRatio_.get(), nearPlane_.get(), farPlane_.get());
}

void CameraProperty::updateViewMatrix() {
   viewMatrix_ = glm::lookAt(lookFrom_.get(), lookTo_.get(), lookUp_.get());
}

void CameraProperty::invalidate(PropertyOwner::InvalidationLevel invalidationLevel) {    
    Processor* owner = dynamic_cast<Processor*>(getOwner());
    if (owner) owner->invalidate(invalidationLevel);
}

void CameraProperty::serialize(IvwSerializer& s) const {
    Property::serialize(s) ;
    s.serialize("lookFrom", lookFrom_);
    s.serialize("lookTo", lookTo_);
    s.serialize("lookUp", lookUp_);
    s.serialize("fovy", fovy_);
    s.serialize("aspectRatio", aspectRatio_);
    s.serialize("nearPlane", nearPlane_);
    s.serialize("farPlane", farPlane_);
}

void CameraProperty::deserialize(IvwDeserializer& d) {
    Property::deserialize(d) ;
    d.deserialize("lookFrom", lookFrom_);
    d.deserialize("lookTo", lookTo_);
    d.deserialize("lookUp", lookUp_);
    d.deserialize("fovy", fovy_);
    d.deserialize("aspectRatio", aspectRatio_);
    d.deserialize("nearPlane", nearPlane_);
    d.deserialize("farPlane", farPlane_);
    updateViewMatrix();
    updateProjectionMatrix();
}

} // namespace
