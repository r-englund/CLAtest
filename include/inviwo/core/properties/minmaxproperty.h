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
 * Contact: Peter Steneteg
 *
 *********************************************************************************/

#ifndef IVW_MINMAXPROPERTY_H
#define IVW_MINMAXPROPERTY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/properties/templateproperty.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <algorithm>

namespace inviwo {

template <typename T>
class MinMaxProperty : public TemplateProperty<glm::detail::tvec2<T, glm::defaultp> > {
public:
    InviwoPropertyInfo();

    MinMaxProperty(
        std::string identifier, std::string displayName, T valueMin = Defaultvalues<T>::getMin(),
        T valueMax = Defaultvalues<T>::getMax(), T rangeMin = Defaultvalues<T>::getMin(),
        T rangeMax = Defaultvalues<T>::getMax(), T increment = Defaultvalues<T>::getInc(),
        T minSeperation = 0,
        PropertyOwner::InvalidationLevel invalidationLevel = PropertyOwner::INVALID_OUTPUT,
        PropertySemantics semantics = PropertySemantics::Default);

    MinMaxProperty(const MinMaxProperty& rhs);
    MinMaxProperty& operator=(const MinMaxProperty& that);
    virtual MinMaxProperty* clone() const;
    virtual ~MinMaxProperty();

    T getRangeMin() const;
    T getRangeMax() const;
    T getIncrement() const;
    T getMinSeparation() const;

    glm::detail::tvec2<T, glm::defaultp> getRange() const;

    virtual void set(const glm::detail::tvec2<T, glm::defaultp>& value);
    virtual void set(const Property* srcProperty);

    void setRangeMin(const T& value);
    void setRangeMax(const T& value);
    void setIncrement(const T& value);
    void setMinSeparation(const T& value);

    void setRange(const glm::detail::tvec2<T, glm::defaultp>& value);

    virtual void setCurrentStateAsDefault();
    virtual void resetToDefaultState();

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& s);

protected:
    void validateValues();

private:
    glm::detail::tvec2<T, glm::defaultp> range_;
    glm::detail::tvec2<T, glm::defaultp> defaultRange_;
    T increment_;
    T defaultIncrement_;
    T minSeparation_;
    T defaultMinSeparation_;
};

typedef MinMaxProperty<float> FloatMinMaxProperty;
typedef MinMaxProperty<double> DoubleMinMaxProperty;
typedef MinMaxProperty<int> IntMinMaxProperty;

template <typename T> PropertyClassIdentifier(MinMaxProperty<T>, "org.inviwo." + Defaultvalues<T>::getName() + "MinMaxProperty" );

template <typename T>
MinMaxProperty<T>::MinMaxProperty(std::string identifier, std::string displayName, T valueMin,
                                  T valueMax, T rangeMin, T rangeMax, T increment, T minSeparation,
                                  PropertyOwner::InvalidationLevel invalidationLevel,
                                  PropertySemantics semantics)
    : TemplateProperty<glm::detail::tvec2<T, glm::defaultp> >(
          identifier, displayName, glm::detail::tvec2<T, glm::defaultp>(valueMin, valueMax),
          invalidationLevel, semantics)
    , range_(glm::detail::tvec2<T, glm::defaultp>(rangeMin, rangeMax))
    , defaultRange_(glm::detail::tvec2<T, glm::defaultp>(rangeMin, rangeMax))
    , increment_(increment)
    , defaultIncrement_(increment)
    , minSeparation_(minSeparation)
    , defaultMinSeparation_(minSeparation) {}

template <typename T>
MinMaxProperty<T>::MinMaxProperty(const MinMaxProperty<T>& rhs)
    : TemplateProperty<glm::detail::tvec2<T, glm::defaultp> >(rhs)  
    , range_(rhs.range_)
    , defaultRange_(rhs.defaultRange_)
    , increment_(rhs.increment_)
    , defaultIncrement_(rhs.defaultIncrement_)
    , minSeparation_(rhs.minSeparation_)
    , defaultMinSeparation_(rhs.defaultMinSeparation_) {
 }

template <typename T>
MinMaxProperty<T>& MinMaxProperty<T>::operator=(const MinMaxProperty<T>& that) {
    if (this != &that) {
        TemplateProperty<glm::detail::tvec2<T, glm::defaultp> >::operator=(that);
        range_ = that.range_;
        defaultRange_ = that.defaultRange_;
        increment_ = that.increment_;
        defaultIncrement_ = that.defaultIncrement_;
        minSeparation_ = that.minSeparation_;
        defaultMinSeparation_ = that.defaultMinSeparation_;
    }
    return *this;
}

template <typename T>
MinMaxProperty<T>* MinMaxProperty<T>::clone() const {
    return new MinMaxProperty<T>(*this);
}

template <typename T>
MinMaxProperty<T>::~MinMaxProperty() {}


template <typename T>
T MinMaxProperty<T>::getRangeMin() const {
    return range_.x;
}

template <typename T>
T MinMaxProperty<T>::getRangeMax() const {
    return range_.y;
}

template <typename T>
T MinMaxProperty<T>::getIncrement() const {
    return increment_;
}

template <typename T>
T inviwo::MinMaxProperty<T>::getMinSeparation() const {
    return minSeparation_;
}

template <typename T>
glm::detail::tvec2<T, glm::defaultp> MinMaxProperty<T>::getRange() const {
    return range_;
}

template <typename T>
void MinMaxProperty<T>::set(const glm::detail::tvec2<T, glm::defaultp>& value) {
    TemplateProperty<glm::detail::tvec2<T, glm::defaultp> >::value_ = value;
    validateValues();
}

template <typename T>
void inviwo::MinMaxProperty<T>::set(const Property* srcProperty) {
    const MinMaxProperty<T>* templatedSrcProp = dynamic_cast<const MinMaxProperty<T>*>(srcProperty);
    if (templatedSrcProp) {
        this->range_ = templatedSrcProp->range_;
        this->defaultRange_ = templatedSrcProp->defaultRange_;
        this->increment_ = templatedSrcProp->increment_;
        this->defaultIncrement_ = templatedSrcProp->defaultIncrement_;
        this->minSeparation_ = templatedSrcProp->minSeparation_;
        this->defaultMinSeparation_ = templatedSrcProp->defaultMinSeparation_;

    } 

    TemplateProperty<glm::detail::tvec2<T, glm::defaultp> >::set(srcProperty);
}

template <typename T>
void MinMaxProperty<T>::setRangeMin(const T& value) {
    if (TemplateProperty<glm::detail::tvec2<T, glm::defaultp> >::value_.x < value ||
        TemplateProperty<glm::detail::tvec2<T, glm::defaultp> >::value_.x == range_.x)
        TemplateProperty<glm::detail::tvec2<T, glm::defaultp> >::value_.x = value;

    range_.x = value;
    TemplateProperty<glm::detail::tvec2<T, glm::defaultp> >::propertyModified();
}

template <typename T>
void MinMaxProperty<T>::setRangeMax(const T& value) {
    if(range_.y == value)
        return;

    if (TemplateProperty<glm::detail::tvec2<T, glm::defaultp> >::value_.y > value ||
        TemplateProperty<glm::detail::tvec2<T, glm::defaultp> >::value_.y == range_.y) {
        TemplateProperty<glm::detail::tvec2<T, glm::defaultp> >::value_.y = value;
    }

    range_.y = value;
    TemplateProperty<glm::detail::tvec2<T, glm::defaultp> >::propertyModified();
}

template <typename T>
void MinMaxProperty<T>::setIncrement(const T& value) {
    if(increment_ == value)
        return;
    increment_ = value;
    TemplateProperty<glm::detail::tvec2<T, glm::defaultp> >::propertyModified();
}

template <typename T>
void inviwo::MinMaxProperty<T>::setMinSeparation(const T& value) {
    if (minSeparation_ == value)
        return;
    minSeparation_ = value;
    validateValues();
}

template <typename T>
void MinMaxProperty<T>::setRange(const glm::detail::tvec2<T, glm::defaultp>& value) {
    setRangeMin(value.x);
    setRangeMax(value.y);
}


template <typename T>
void MinMaxProperty<T>::resetToDefaultState() {
    range_ = defaultRange_;
    increment_ = defaultIncrement_;
    minSeparation_ = defaultMinSeparation_;
    TemplateProperty<glm::detail::tvec2<T, glm::defaultp> >::resetToDefaultState();
}

template <typename T>
void MinMaxProperty<T>::setCurrentStateAsDefault() {
    TemplateProperty<glm::detail::tvec2<T, glm::defaultp> >::setCurrentStateAsDefault();
    defaultRange_ = range_;
    defaultIncrement_ = increment_;
    defaultMinSeparation_ = minSeparation_;
}

template <typename T>
void MinMaxProperty<T>::serialize(IvwSerializer& s) const {
    TemplateProperty<glm::detail::tvec2<T, glm::defaultp> >::serialize(s);

    if (range_ != defaultRange_) {
        s.serialize("range", range_);
    }
    if (increment_ != defaultIncrement_) {
        s.serialize("increment", MinMaxProperty<T>::getIncrement());
    }
    if (minSeparation_ != defaultMinSeparation_) {
        s.serialize("minSeparation", minSeparation_);
    }
}

template <typename T>
void MinMaxProperty<T>::deserialize(IvwDeserializer& d) {
    d.deserialize("range", range_);
    d.deserialize("increment", increment_);
    d.deserialize("minSeparation", minSeparation_);
    TemplateProperty<glm::detail::tvec2<T, glm::defaultp> >::deserialize(d);
}

template <typename T>
void MinMaxProperty<T>::validateValues() {
    glm::detail::tvec2<T, glm::defaultp>& val =
        TemplateProperty<glm::detail::tvec2<T, glm::defaultp> >::value_;

    if (val.x > val.y) std::swap(val.x, val.y);
    // check bounds
    val.x = std::min(std::max(val.x, range_.x), range_.y);
    val.y = std::min(std::max(val.y, range_.x), range_.y);

    // check whether updated min/max values are separated properly, i.e. > minSeparation_
    if (std::abs(val.y - val.x) < minSeparation_ - glm::epsilon<T>()) {
        // adjust max value if possible, i.e. less equal than max range
        if (val.x + minSeparation_ < range_.y + glm::epsilon<T>()) {
            val.y = std::max(val.x + minSeparation_, val.y);
        } else {
            // otherwise adjust min value
            val.y = range_.y;
            val.x = range_.y - minSeparation_;
        }
    }
    TemplateProperty<glm::detail::tvec2<T, glm::defaultp> >::propertyModified();
}

}  // namespace

#endif  // IVW_MINMAXPROPERTY_H
