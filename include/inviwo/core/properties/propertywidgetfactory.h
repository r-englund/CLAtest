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

#ifndef IVW_PROPERTYWIDGETFACTORY_H
#define IVW_PROPERTYWIDGETFACTORY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/properties/propertywidget.h>
#include <inviwo/core/properties/propertywidgetfactoryobject.h>
#include <inviwo/core/util/factory.h>
#include <inviwo/core/util/singleton.h>


namespace inviwo {

class IVW_CORE_API PropertyWidgetFactory : public Factory,
                                           public Singleton<PropertyWidgetFactory> {

public:
    PropertyWidgetFactory();
    ~PropertyWidgetFactory();

    PropertyWidget* create(Property* property);

    void registerObject(PropertyWidgetFactoryObject* propertyWidget);
    virtual IvwSerializable* create(std::string className) const;
    virtual bool isValidType(std::string className) const;
    
    std::vector<PropertySemantics> getSupportedSemanicsForProperty(Property* property);

    typedef std::multimap<std::string, PropertyWidgetFactoryObject*> WidgetMap;
private:
    mutable WidgetMap widgetMap_;
};

} // namespace

#endif // IVW_PROPERTYWIDGETFACTORY_H
