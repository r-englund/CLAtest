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

#ifndef IVW_PROPERTYWIDGETFACTORYQT_H
#define IVW_PROPERTYWIDGETFACTORYQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/core/properties/property.h>
#include <inviwo/core/util/singleton.h>

#include <inviwo/qt/widgets/properties/propertywidgetqt.h>

namespace inviwo {

class IVW_QTWIDGETS_API PropertyWidgetFactoryQt : public Singleton<PropertyWidgetFactoryQt> {
//TODO: derive from Factory class

public:
    PropertyWidgetFactoryQt();
    PropertyWidgetQt* create(Property* property);
};

} // namespace

#endif // IVW_PROPERTYWIDGETFACTORYQT_H