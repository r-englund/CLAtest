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
 * Main file authors: Sathish Kottravel, Peter Steneteg
 *
 *********************************************************************************/

#include <inviwo/core/properties/scalarproperties.h>

namespace inviwo {

FloatProperty::FloatProperty(std::string identifier, std::string displayName, float value,
                             float minValue, float maxValue, float increment,
                             PropertyOwner::InvalidationLevel invalidationLevel,
                             PropertySemantics semantics)
    : OrdinalProperty<float>(identifier, displayName, value, minValue, maxValue, increment,
                             invalidationLevel, semantics)
{}

int FloatProperty::getVariantType() {
    return Variant::VariantTypeFloat;
}

Variant FloatProperty::getVariant() {
    return Variant(get());
}

void  FloatProperty::setVariant(const Variant& val) {
    if (val.canConvert(getVariantType()))
        set(val.getFloat());
}

void FloatProperty::serialize(IvwSerializer& s) const {
    Property::serialize(s) ;
    s.serialize("value", get());
    s.serialize("minvalue", OrdinalProperty<float>::getMinValue());
    s.serialize("maxvalue", OrdinalProperty<float>::getMaxValue());
    s.serialize("increment", OrdinalProperty<float>::getIncrement());
}

void FloatProperty::deserialize(IvwDeserializer& d) {
    Property::deserialize(d) ;
    float value;
    d.deserialize("value", value);
    set(value);
    d.deserialize("minvalue",  value);
    OrdinalProperty<float>::setMinValue(value);
    d.deserialize("maxvalue", value);
    OrdinalProperty<float>::setMaxValue(value);
    d.deserialize("increment", value);
    OrdinalProperty<float>::setIncrement(value);
}


DoubleProperty::DoubleProperty(std::string identifier, std::string displayName, double value,
                               double minValue, double maxValue, double increment,
                               PropertyOwner::InvalidationLevel invalidationLevel,
                               PropertySemantics semantics)
    : OrdinalProperty<double>(identifier, displayName, value, minValue, maxValue, increment,
                              invalidationLevel, semantics) {}

int DoubleProperty::getVariantType() {
    return Variant::VariantTypeDouble;
}

Variant DoubleProperty::getVariant() {
    return Variant(get());
}

void  DoubleProperty::setVariant(const Variant& val) {
    if (val.canConvert(getVariantType()))
        set(val.getDouble());
}

void DoubleProperty::serialize(IvwSerializer& s) const {
    Property::serialize(s);
    s.serialize("value", get());
    s.serialize("minvalue", OrdinalProperty<double>::getMinValue());
    s.serialize("maxvalue", OrdinalProperty<double>::getMaxValue());
    s.serialize("increment", OrdinalProperty<double>::getIncrement());
}

void DoubleProperty::deserialize(IvwDeserializer& d) {
    Property::deserialize(d);
    double value;
    d.deserialize("value", value);
    set(value);
    d.deserialize("minvalue", value);
    OrdinalProperty<double>::setMinValue(value);
    d.deserialize("maxvalue", value);
    OrdinalProperty<double>::setMaxValue(value);
    d.deserialize("increment", value);
    OrdinalProperty<double>::setIncrement(value);
}


IntProperty::IntProperty(std::string identifier, std::string displayName, int value,
                         int minValue, int maxValue, int increment,
                         PropertyOwner::InvalidationLevel invalidationLevel,
                         PropertySemantics semantics)
    : OrdinalProperty<int>(identifier, displayName, value, minValue, maxValue, increment,
                           invalidationLevel, semantics)
{}

int IntProperty::getVariantType() {
    return Variant::VariantTypeInteger;
}

Variant IntProperty::getVariant() {
    return Variant(get());
}

void  IntProperty::setVariant(const Variant& val) {
    if (val.canConvert(getVariantType()))
        set(val.getInt());
}

void IntProperty::serialize(IvwSerializer& s) const {
    Property::serialize(s) ;
    s.serialize("value", get());
    s.serialize("minvalue", OrdinalProperty<int>::getMinValue());
    s.serialize("maxvalue", OrdinalProperty<int>::getMaxValue());
    s.serialize("increment", OrdinalProperty<int>::getIncrement());
}

void IntProperty::deserialize(IvwDeserializer& d) {
    Property::deserialize(d) ;
    int value;
    d.deserialize("value", value);
    set(value);
    d.deserialize("minvalue",  value);
    OrdinalProperty<int>::setMinValue(value);
    d.deserialize("maxvalue", value);
    OrdinalProperty<int>::setMaxValue(value);
    d.deserialize("increment", value);
    OrdinalProperty<int>::setIncrement(value);
}

} // namespace
