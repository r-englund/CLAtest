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

#ifndef IVW_PROPERTYWIDGET_H
#define IVW_PROPERTYWIDGET_H

#include <inviwo/core/common/inviwocoredefine.h>

namespace inviwo {

class IVW_CORE_API PropertyWidget {

public:
    PropertyWidget();
    PropertyWidget(Property* property);

    virtual ~PropertyWidget();

    virtual void updateFromProperty() = 0;
    virtual void showWidget() = 0;
    virtual void hideWidget() = 0;

    virtual Property* getProperty();
    virtual void setProperty(Property* property);

protected:
    Property* property_; //< Non owning reference to a property.
};

} // namespace

#endif // IVW_PROPERTYWIDGET_H