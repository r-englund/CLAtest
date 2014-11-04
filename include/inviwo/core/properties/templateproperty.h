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
 * Contact: Peter Steneteg
 *
 *********************************************************************************/

#ifndef IVW_TEMPLATEPROPERTY_H
#define IVW_TEMPLATEPROPERTY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/properties/property.h>

namespace inviwo {

template <typename T>
class TemplateProperty : public Property {
public:
    typedef T valueType;

    TemplateProperty(
        const std::string& identifier, const std::string& displayName, const T& value = T(),
        PropertyOwner::InvalidationLevel invalidationLevel = PropertyOwner::INVALID_OUTPUT,
        PropertySemantics semantics = PropertySemantics::Default);

    TemplateProperty(const TemplateProperty& rhs);
    TemplateProperty<T>& operator=(const TemplateProperty<T>& that);
    virtual TemplateProperty* clone() const;
    operator T();
    virtual ~TemplateProperty();

    virtual T& get();
    virtual const T& get() const { return value_; };
    virtual void set(const T& value);
    virtual void set(const Property* srcProperty);

    virtual void setCurrentStateAsDefault();
    virtual void resetToDefaultState();

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

protected:
    T value_;
    T defaultValue_;
};

template <typename T>
TemplateProperty<T>* TemplateProperty<T>::clone() const {
    return new TemplateProperty<T>(*this);
}

template<typename T>
TemplateProperty<T>::TemplateProperty(const TemplateProperty<T>& rhs)
    : Property(rhs)
    , value_(rhs.value_)
    , defaultValue_(rhs.defaultValue_) {
}

template<typename T>
TemplateProperty<T>& TemplateProperty<T>::operator=(const TemplateProperty<T>& that) {
    if (this != &that) {
        Property::operator=(that);
        value_ = that.value_;
        defaultValue_ = that.defaultValue_;
    }
    return *this;
}

template<typename T>
TemplateProperty<T>::operator T() {
    return value_;
}

template<typename T>
TemplateProperty<T>::~TemplateProperty() {}

template<typename T>
void inviwo::TemplateProperty<T>::resetToDefaultState() { 
    value_ = defaultValue_;
    Property::resetToDefaultState();
}

template<typename T>
void inviwo::TemplateProperty<T>::setCurrentStateAsDefault() {
    Property::setCurrentStateAsDefault();
    defaultValue_ = value_;
}

template <typename T>
TemplateProperty<T>::TemplateProperty(const std::string &identifier, const std::string &displayName, const T &value,
                                      PropertyOwner::InvalidationLevel invalidationLevel,
                                      PropertySemantics semantics)
    : Property(identifier, displayName, invalidationLevel, semantics)
    , value_(value)
    , defaultValue_(value) {
}

template <typename T>
T& TemplateProperty<T>::get() {
    return value_;
}

template <typename T>
void TemplateProperty<T>::set(const T& value) {
    if (value == value_)
        return;
    value_ = value;
    propertyModified();
}

template <typename T>
void TemplateProperty<T>::set(const Property* srcProperty) {
    const TemplateProperty<T>* templatedSrcProp =
        dynamic_cast<const TemplateProperty<T>*>(srcProperty);
    if (templatedSrcProp) {
        this->value_ = templatedSrcProp->value_;
    }

    Property::set(srcProperty);
}

template <typename T>
void TemplateProperty<T>::serialize(IvwSerializer& s) const {
    Property::serialize(s);
    if (value_ != defaultValue_) {
        s.serialize("value", value_);
    }
}

template <typename T>
void TemplateProperty<T>::deserialize(IvwDeserializer& d) {
    Property::deserialize(d);
    d.deserialize("value", value_);
    propertyModified();
}

} // namespace

#endif // IVW_TEMPLATEPROPERTY_H
