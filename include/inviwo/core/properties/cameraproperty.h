#ifndef IVW_CAMERAPROPERTY_H
#define IVW_CAMERAPROPERTY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/vectorproperties.h>
#include <inviwo/core/properties/scalarproperties.h>
#include <inviwo/core/properties/compositeproperty.h>
#include <inviwo/core/interaction/events/eventlistener.h>

namespace inviwo {

class IVW_CORE_API CameraProperty : public CompositeProperty, public EventListener {

public:
    CameraProperty(std::string identifier, std::string displayName,
                   vec3 eye=vec3(0.0f, 0.0f, -2.0f), vec3 center=vec3(0.0f), vec3 lookUp=vec3(0.0f, 1.0f, 0.0f),
                   PropertyOwner::InvalidationLevel=PropertyOwner::INVALID_OUTPUT,
                   PropertySemantics::Type semantics = PropertySemantics::Default);
    virtual ~CameraProperty();

    vec3 getLookFrom() const { return lookFrom_.get(); }
    void setLookFrom(vec3 lookFrom);
    vec3 getLookTo() const { return lookTo_.get(); }
    void setLookTo(vec3 lookTo);
    vec3 getLookUp() const { return lookUp_.get(); }
    void setLookUp(vec3 lookUp);

    float getDistanceFromOrigin() const;
    vec3 getWorldPosFromWindowCoords(vec3 windowCoords, ivec2 windowSize, mat4 modelMatrix = mat4(1.f)) const;

    void updateViewMatrix();
    void updateProjectionMatrix();
    mat4 viewMatrix() const { return viewMatrix_; }
    mat4 projectionMatrix() const { return projectionMatrix_; }
    mat4 inverseViewMatrix() const { return inverseViewMatrix_; }
    mat4 inverseProjectionMatrix() const { return inverseProjectionMatrix_; }

    void setProjectionMatrix(float fovy, float aspect, float farPlane, float nearPlane);

    void invokeEvent(Event* event);

    void invalidate();
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
    virtual std::string getClassName()  const { return "CameraProperty"; }

private:
    FloatVec3Property lookFrom_;
    FloatVec3Property lookTo_;
    FloatVec3Property lookUp_;
    FloatProperty fovy_;
    FloatProperty aspectRatio_;
    BoolProperty useNearFarDistFromOrigin_;
    FloatProperty farPlane_;
    FloatProperty nearPlane_;

    mat4 viewMatrix_;
    mat4 projectionMatrix_;
    mat4 inverseViewMatrix_;
    mat4 inverseProjectionMatrix_;
};

} // namespace

#endif // IVW_CAMERAPROPERTY_H
