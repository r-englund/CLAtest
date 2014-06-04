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
 * Main file authors: Erik Sundén, Timo Ropinski, Rickard Englund
 *
 *********************************************************************************/

#ifndef IVW_ORDINALPROPERTY_H
#define IVW_ORDINALPROPERTY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/properties/templateproperty.h>
#include <string>
#include <sstream>

namespace inviwo {

template <typename T>
class OrdinalProperty : public TemplateProperty<T> {
public:
    OrdinalProperty(
        std::string identifier, std::string displayName, T value = Defaultvalues<T>::getVal(),
        T minValue = Defaultvalues<T>::getMin(), T maxValue = Defaultvalues<T>::getMax(),
        T increment = Defaultvalues<T>::getInc(),
        PropertyOwner::InvalidationLevel invalidationLevel = PropertyOwner::INVALID_OUTPUT,
        PropertySemantics semantics = PropertySemantics::Default);

    T getMinValue() const;
    T getMaxValue() const;
    T getIncrement() const;

    virtual void set(const T& value);  // Need to implement this to avoid compiler confusion with
                                       // set(const Property*)
    virtual void set(const Property* src);
    void setMinValue(const T& value);
    void setMaxValue(const T& value);
    void setIncrement(const T& value);

    virtual void setCurrentStateAsDefault();
    virtual void resetToDefaultState();

    virtual std::string getClassName() const;

    virtual Variant getVariant();
    virtual void setVariant(const Variant& v);
    virtual int getVariantType();

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

    static uvec2 getDim() { return Defaultvalues<T>::getDim(); }

private:
    T minValue_;
    T maxValue_;
    T increment_;

    T defaultMinValue_;
    T defaultMaxValue_;
    T defaultIncrement_;
};

//Scalar properties
typedef OrdinalProperty<float> FloatProperty;
typedef OrdinalProperty<int> IntProperty;
typedef OrdinalProperty<glm::i64> Int64Property;
typedef OrdinalProperty<double> DoubleProperty;

//Vector properties 
typedef OrdinalProperty<vec2> FloatVec2Property;
typedef OrdinalProperty<vec3> FloatVec3Property;
typedef OrdinalProperty<vec4> FloatVec4Property;

typedef OrdinalProperty<dvec2> DoubleVec2Property;
typedef OrdinalProperty<dvec3> DoubleVec3Property;
typedef OrdinalProperty<dvec4> DoubleVec4Property;

typedef OrdinalProperty<ivec2> IntVec2Property;
typedef OrdinalProperty<ivec3> IntVec3Property;
typedef OrdinalProperty<ivec4> IntVec4Property;

//Matrix properties
typedef OrdinalProperty<mat2> FloatMat2Property;
typedef OrdinalProperty<mat3> FloatMat3Property;
typedef OrdinalProperty<mat4> FloatMat4Property;

typedef OrdinalProperty<dmat2> DoubleMat2Property;
typedef OrdinalProperty<dmat3> DoubleMat3Property;
typedef OrdinalProperty<dmat4> DoubleMat4Property;


template<typename T>
std::string OrdinalProperty<T>::getClassName() const {
    std::stringstream ss;
    ss << Defaultvalues<T>::getName() << "Property";
    return ss.str();
}

template <typename T>
OrdinalProperty<T>::OrdinalProperty(std::string identifier, std::string displayName, T value,
                                    T minValue, T maxValue, T increment,
                                    PropertyOwner::InvalidationLevel invalidationLevel,
                                    PropertySemantics semantics)
    : TemplateProperty<T>(identifier, displayName, value, invalidationLevel, semantics)
    , minValue_(minValue)
    , maxValue_(maxValue)
    , increment_(increment)
    , defaultMinValue_(minValue)
    , defaultMaxValue_(maxValue)
    , defaultIncrement_(increment) {
}


template <typename T>
void OrdinalProperty<T>::set(const T& value) {
    if(TemplateProperty<T>::value_ != value){
        TemplateProperty<T>::set(value);
    }
}

template <typename T>
void OrdinalProperty<T>::set(const Property* srcProperty) {
    const OrdinalProperty<T>* templatedSrcProp =
        dynamic_cast<const OrdinalProperty<T>*>(srcProperty);
    if (templatedSrcProp) {
        this->minValue_ = templatedSrcProp->minValue_;
        this->maxValue_ = templatedSrcProp->maxValue_;
        this->increment_ = templatedSrcProp->increment_;
        this->defaultMinValue_ = templatedSrcProp->defaultMinValue_;
        this->defaultMaxValue_ = templatedSrcProp->defaultMaxValue_;
        this->defaultIncrement_ = templatedSrcProp->defaultIncrement_;
    } else
        this->setVariant(const_cast<Property*>(srcProperty)->getVariant());

    TemplateProperty<T>::set(srcProperty);
}

template <typename T>
T OrdinalProperty<T>::getMinValue() const {
    return minValue_;
}

template <typename T>
T OrdinalProperty<T>::getMaxValue() const {
    return maxValue_;
}

template <typename T>
T OrdinalProperty<T>::getIncrement() const {
    return increment_;
}

template <typename T>
void OrdinalProperty<T>::setMinValue(const T& value) {
    minValue_ = value;
    Property::propertyModified();
    
}

template <typename T>
void OrdinalProperty<T>::setMaxValue(const T& value) {
    maxValue_ = value;
    Property::propertyModified();
}

template <typename T>
void OrdinalProperty<T>::setIncrement(const T& value) {
    increment_ = value;
    Property::propertyModified();
}

template <typename T>
Variant OrdinalProperty<T>::getVariant() {
    return Variant(TemplateProperty<T>::value_);
}

template <typename T>
void OrdinalProperty<T>::setVariant(const Variant& v) {
    if (v.canConvert(getVariantType())) {
        //Input variant type can be different from property variant type
        Variant currentVariant(getVariant());
        //Invokes conversion during assignment operation
        currentVariant = v;
        //Get from variants and set
        this->set(currentVariant.get<T>());
    }
}

template <typename T>
int OrdinalProperty<T>::getVariantType() {
    return getVariant().getType();
}

template<typename T>
void inviwo::OrdinalProperty<T>::resetToDefaultState() {   
    minValue_ = defaultMinValue_;
    maxValue_ = defaultMaxValue_;
    increment_ = defaultIncrement_;
    TemplateProperty<T>::resetToDefaultState();
}

template<typename T>
void inviwo::OrdinalProperty<T>::setCurrentStateAsDefault() {
    TemplateProperty<T>::setCurrentStateAsDefault();
    defaultMinValue_ = minValue_;
    defaultMaxValue_ = maxValue_;
    defaultIncrement_ = increment_;
}


template<typename T>
void OrdinalProperty<T>::serialize(IvwSerializer& s) const {
    if (minValue_ != defaultMinValue_) {
        s.serialize("minvalue", minValue_);
    }
    if (maxValue_ != defaultMaxValue_) {
        s.serialize("maxvalue", maxValue_);
    }
    if (increment_ != defaultIncrement_) {
        s.serialize("increment", increment_);
    }
    
    TemplateProperty<T>::serialize(s);
}


template<typename T>
void OrdinalProperty<T>::deserialize(IvwDeserializer& d) {
    d.deserialize("minvalue", minValue_);
    d.deserialize("maxvalue", maxValue_);
    d.deserialize("increment", increment_);
    
    TemplateProperty<T>::deserialize(d);
}

} // namespace

#endif // IVW_ORDINALPROPERTY_H
