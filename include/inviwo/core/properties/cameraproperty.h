/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

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
                   Inport* inport = NULL,
                   PropertyOwner::InvalidationLevel=PropertyOwner::INVALID_OUTPUT,
                   PropertySemantics semantics = PropertySemantics::Default);
    virtual ~CameraProperty();

    vec3 getLookFrom() const { return lookFrom_.get(); }
    void setLookFrom(vec3 lookFrom);
    vec3 getLookTo() const { return lookTo_.get(); }
    void setLookTo(vec3 lookTo);
    vec3 getLookUp() const { return lookUp_.get(); }
    void setLookUp(vec3 lookUp);
    vec3 getLookRight() const { return lookRight_; }

    void setLook(vec3 lookFrom, vec3 lookTo, vec3 lookUp);

    float getNearPlaneDist() const;
    float getFarPlaneDist() const;

    vec3 getWorldPosFromNormalizedDeviceCoords(vec3 ndcCoords) const;

    void updateViewMatrix();
    void updateProjectionMatrix();
    const mat4& viewMatrix() const { return viewMatrix_; }
    const mat4& projectionMatrix() const { return projectionMatrix_; }
    const mat4& inverseViewMatrix() const { return inverseViewMatrix_; }
    const mat4& inverseProjectionMatrix() const { return inverseProjectionMatrix_; }

    void setProjectionMatrix(float fovy, float aspect, float farPlane, float nearPlane);

    void invokeEvent(Event* event);

    STARTCLANGIGNORE("-Woverloaded-virtual")
    void invalidate();
    ENDCLANGIGNORE
    void propertyModified(){};

    void lockInvalidation() { lockInvalidation_ = true; }
    void unlockInvalidation() { lockInvalidation_ = false; }
    bool isInvalidationLocked() { return lockInvalidation_; }

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
    virtual std::string getClassName()  const { return "CameraProperty"; }

    void setInport(Inport* inport);
    void fitCameraToVolume( const Volume* volume);
    void fitCameraToGeomtry( const Geometry* volume);
    void fitWithBasis(const mat3 &basis);
    void inportChanged();
private:
    FloatVec3Property lookFrom_;
    FloatVec3Property lookTo_;
    FloatVec3Property lookUp_;
    FloatProperty fovy_;
    FloatProperty aspectRatio_;
    FloatProperty farPlane_;
    FloatProperty nearPlane_;

    vec3 lookRight_;
    mat4 viewMatrix_;
    mat4 projectionMatrix_;
    mat4 inverseViewMatrix_;
    mat4 inverseProjectionMatrix_;

    bool lockInvalidation_;

    Inport* inport_;
    const SpatialEntity<3>* data_;
    mat3 oldBasis_;
};

} // namespace

#endif // IVW_CAMERAPROPERTY_H
