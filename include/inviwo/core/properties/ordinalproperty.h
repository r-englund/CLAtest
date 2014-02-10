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

namespace inviwo {

template<typename T>
class OrdinalProperty : public TemplateProperty<T> {

public:
    OrdinalProperty(std::string identifier, std::string displayName, T value,
                    T minValue, T maxValue, T increment, PropertyOwner::InvalidationLevel invalidationLevel,
                    PropertySemantics semantics = PropertySemantics::Default);

    T getMinValue() const;
    T getMaxValue() const;
    T getIncrement() const;

    virtual void set(const T& value) { TemplateProperty<T>::set(value); } // Need to implement this to avoid compiler confusion with set(const Property*)
    virtual void set(const Property* src);
    void setMinValue(const T& value);
    void setMaxValue(const T& value);
    void setIncrement(const T& value);

    virtual Variant getVariant();
    virtual void setVariant(const Variant& v);
    virtual int getVariantType();


private:
    T minValue_;
    T maxValue_;
    T increment_;
};


template <typename T>
OrdinalProperty<T>::OrdinalProperty(std::string identifier, std::string displayName, T value,
                                    T minValue, T maxValue, T increment,
                                    PropertyOwner::InvalidationLevel invalidationLevel,
                                    PropertySemantics semantics)
    : TemplateProperty<T>(identifier, displayName, value, invalidationLevel, semantics),
      minValue_(minValue),
      maxValue_(maxValue),
      increment_(increment)
{}

template <typename T>
void OrdinalProperty<T>::set(const Property* srcProperty) {
    const OrdinalProperty<T>* templatedSrcProp = dynamic_cast<const OrdinalProperty<T>*>(srcProperty);

    if (templatedSrcProp) {
        minValue_ = templatedSrcProp->getMinValue();
        maxValue_ = templatedSrcProp->getMaxValue();
        increment_ = templatedSrcProp->getIncrement();
        this->value_ = templatedSrcProp->get();
    } else
        this->setVariant(const_cast<Property*>(srcProperty)->getVariant());

    propertyModified();
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
}

template <typename T>
void OrdinalProperty<T>::setMaxValue(const T& value) {
    maxValue_ = value;
}

template <typename T>
void OrdinalProperty<T>::setIncrement(const T& value) {
    increment_ = value;
}

template <typename T>
Variant OrdinalProperty<T>::getVariant() {
    return Variant(TemplateProperty<T>::value_);
}

template <typename T>
void OrdinalProperty<T>::setVariant(const Variant& v) {
    if (v.canConvert(getVariantType()))
        this->set(v.get<T>());
}

template <typename T>
int OrdinalProperty<T>::getVariantType() {
    return getVariant().getType();
}


} // namespace

#endif // IVW_ORDINALPROPERTY_H
