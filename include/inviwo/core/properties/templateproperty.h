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

#ifndef IVW_TEMPLATEPROPERTY_H
#define IVW_TEMPLATEPROPERTY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/properties/property.h>
#include <inviwo/core/util/variant.h>

namespace inviwo {

template<typename T>
class TemplateProperty : public Property {

public:

    TemplateProperty(std::string identifier, std::string displayName, T value,
                     PropertyOwner::InvalidationLevel invalidationLevel = PropertyOwner::INVALID_OUTPUT,
                     PropertySemantics semantics = PropertySemantics::Default);

    virtual T& get();
	virtual const T& get() const { return value_; };
    virtual void set(const T& value);
    virtual void set(const Property* srcProperty);

protected:
    T value_;
};

template <typename T>
TemplateProperty<T>::TemplateProperty(std::string identifier, std::string displayName, T value,
                                      PropertyOwner::InvalidationLevel invalidationLevel,
                                      PropertySemantics semantics)
: Property(identifier, displayName, invalidationLevel, semantics),
value_(value)
{}

template <typename T>
T& TemplateProperty<T>::get() {
    return value_;
}

template <typename T>
void TemplateProperty<T>::set(const T& value) {
    value_ = value;
	propertyModified();    
}

template <typename T>
void TemplateProperty<T>::set(const Property* srcProperty) {
    const TemplateProperty<T>* templatedSrcProp = dynamic_cast<const TemplateProperty<T>*>(srcProperty);
    if (templatedSrcProp) 
        this->value_ = templatedSrcProp->get();
    else        
        this->setVariant(const_cast<Property*>(srcProperty)->getVariant());
	propertyModified();    
}


} // namespace

#endif // IVW_TEMPLATEPROPERTY_H
