/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Daniel Jönsson
 *
 **********************************************************************/

#ifndef IVW_TEMPLATE_RESOURCE_H
#define IVW_TEMPLATE_RESOURCE_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/properties/property.h>

namespace inviwo {

/** \class TemplateResource
*
* A TemplateResource is a container for data of type T.
*
* @see Resource
* @see ResourceManager
*/
template<typename T>
class TemplateResource : public Resource {

public:
    /**
     * Resource will take ownership of data and then delete it upon destruction.
     *
     * @param value Pointer to allocated data.
     */
    TemplateResource(const std::string& identifier, T* value);

    virtual ~TemplateResource() { delete value_; }

    virtual T* getData() { return value_; };
    virtual const T* getData() const { return value_; };

    virtual const std::string& getIdentifier() const { return identifier_; };

protected:
    std::string identifier_;
    T* value_;
};

template <typename T>
TemplateResource<T>::TemplateResource(const std::string& identifier, T* value)
    : Resource(), identifier_(identifier), value_(value)
{}


} // namespace

#endif // IVW_TEMPLATE_RESOURCE_H
