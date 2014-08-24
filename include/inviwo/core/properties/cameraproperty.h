/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Main file authors: Erik Sundén, Timo Ropinski
 *
 *********************************************************************************/

#ifndef IVW_CAMERAPROPERTY_H
#define IVW_CAMERAPROPERTY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/geometry/geometry.h>
#include <inviwo/core/datastructures/volume/volume.h>
#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <inviwo/core/properties/compositeproperty.h>
#include <inviwo/core/ports/inport.h>
#include <inviwo/core/interaction/events/eventlistener.h>

namespace inviwo {

class IVW_CORE_API CameraProperty : public CompositeProperty, public EventListener {

public:
    CameraProperty(std::string identifier,
                   std::string displayName,
                   vec3 eye = vec3(0.0f, 0.0f, -2.0f),
                   vec3 center = vec3(0.0f),
                   vec3 lookUp = vec3(0.0f, 1.0f, 0.0f),
                   Inport* inport = NULL,
                   PropertyOwner::InvalidationLevel=PropertyOwner::INVALID_OUTPUT,
                   PropertySemantics semantics = PropertySemantics::Default);
    virtual ~CameraProperty();

    void resetCamera();

    vec3& getLookFrom() { return lookFrom_.get(); }
    const vec3& getLookFrom() const { return lookFrom_.get(); }
    void setLookFrom(vec3 lookFrom);
    vec3& getLookTo() { return lookTo_.get(); }
    const vec3& getLookTo() const { return lookTo_.get(); }
    void setLookTo(vec3 lookTo);
    vec3& getLookUp() { return lookUp_.get(); }
    const vec3& getLookUp() const { return lookUp_.get(); }
    void setLookUp(vec3 lookUp);
    vec3 getLookRight() const { return lookRight_; }

    float getFovy() const { return fovy_.get(); }
    void setFovy(float fovy);

    float getAspectRatio() const { return aspectRatio_.get(); }

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


    virtual void invalidate(PropertyOwner::InvalidationLevel invalidationLevel,
                            Property* modifiedProperty = 0);

    // Local camera invalidation
    // Use lock and unlock to set several camera properties without casing evaluation,
    // then call invalidate().
    void invalidate();
    void lockInvalidation() { lockInvalidation_ = true; }
    void unlockInvalidation() { lockInvalidation_ = false; }
    bool isInvalidationLocked() { return lockInvalidation_; }

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
    virtual std::string getClassName() const { return "CameraProperty"; }

    void setInport(Inport* inport);
    void fitWithBasis(const mat3& basis);
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

    const glm::vec3 initialEye_;
    const glm::vec3 initialCenter_;
    const glm::vec3 initialUp_;
    const float initialFovy_;
};

} // namespace

#endif // IVW_CAMERAPROPERTY_H
