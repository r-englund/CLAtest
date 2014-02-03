 /*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
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
 * Main file author: Alexander Johansson
 *
 *********************************************************************************/

#include "inviwo/core/properties/matrixproperties.h"

namespace inviwo {

FloatMat2Property::FloatMat2Property(std::string identifier, std::string displayName, mat2 value,
                                     mat2 minValue, mat2 maxValue, mat2 increment,
                                     PropertyOwner::InvalidationLevel invalidationLevel,
                                     PropertySemantics semantics)
    : OrdinalProperty<mat2>(identifier,displayName,value, minValue, maxValue, increment, invalidationLevel, semantics)
{}

void FloatMat2Property::serialize(IvwSerializer& s) const {
    Property::serialize(s);
    s.serialize("value",get());
    s.serialize("minvalue", OrdinalProperty<mat2>::getMinValue());
    s.serialize("maxvalue", OrdinalProperty<mat2>::getMaxValue());
    s.serialize("increment", OrdinalProperty<mat2>::getIncrement());
}

void FloatMat2Property::deserialize(IvwDeserializer& d) {
    Property::deserialize(d);
    mat2 value;
    d.deserialize("value",value);
    set(value);
    d.deserialize("minvalue", value);
    OrdinalProperty<mat2>::setMinValue(value);
    d.deserialize("maxvalue",value);
    OrdinalProperty<mat2>::setMaxValue(value);
    d.deserialize("increment", value);
    OrdinalProperty<mat2>::setIncrement(value);
}

FloatMat3Property::FloatMat3Property(std::string identifier, std::string displayName, mat3 value,
                                     mat3 minValue, mat3 maxValue, mat3 increment,
                                     PropertyOwner::InvalidationLevel invalidationLevel,
                                     PropertySemantics semantics)
    : OrdinalProperty<mat3>(identifier,displayName,value, minValue, maxValue, increment, invalidationLevel, semantics)
{}

void FloatMat3Property::serialize(IvwSerializer& s) const {
    Property::serialize(s);
    s.serialize("value",get());
    s.serialize("minvalue", OrdinalProperty<mat3>::getMinValue());
    s.serialize("maxvalue", OrdinalProperty<mat3>::getMaxValue());
    s.serialize("increment", OrdinalProperty<mat3>::getIncrement());
}

void FloatMat3Property::deserialize(IvwDeserializer& d) {
    Property::deserialize(d);
    mat3 value;
    d.deserialize("value",value);
    set(value);
    d.deserialize("minvalue", value);
    OrdinalProperty<mat3>::setMinValue(value);
    d.deserialize("maxvalue",value);
    OrdinalProperty<mat3>::setMaxValue(value);
    d.deserialize("increment", value);
    OrdinalProperty<mat3>::setIncrement(value);
}

FloatMat4Property::FloatMat4Property(std::string identifier, std::string displayName, mat4 value,
                                     mat4 minValue, mat4 maxValue, mat4 increment,
                                     PropertyOwner::InvalidationLevel invalidationLevel,
                                     PropertySemantics semantics)
    : OrdinalProperty<mat4>(identifier,displayName,value, minValue, maxValue, increment, invalidationLevel, semantics)
{}

void FloatMat4Property::serialize(IvwSerializer& s) const {
    Property::serialize(s);
    s.serialize("value",get());
    s.serialize("minvalue", OrdinalProperty<mat4>::getMinValue());
    s.serialize("maxvalue", OrdinalProperty<mat4>::getMaxValue());
    s.serialize("increment", OrdinalProperty<mat4>::getIncrement());
}

void FloatMat4Property::deserialize(IvwDeserializer& d) {
    Property::deserialize(d);
    mat4 value;
    d.deserialize("value",value);
    set(value);
    d.deserialize("minvalue", value);
    OrdinalProperty<mat4>::setMinValue(value);
    d.deserialize("maxvalue",value);
    OrdinalProperty<mat4>::setMaxValue(value);
    d.deserialize("increment", value);
    OrdinalProperty<mat4>::setIncrement(value);
}

}//namespace