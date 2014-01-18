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

#ifndef IVW_COMPOSITEPROPERTYWIDGETQT_H
#define IVW_COMPOSITEPROPERTYWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/properties/collapsivegroupboxwidgetqt.h>
#include <QLineEdit>
#include <QToolButton>

#include <inviwo/qt/widgets/properties/propertywidgetqt.h>

#include <inviwo/core/properties/compositeproperty.h>

namespace inviwo {

class IVW_QTWIDGETS_API CompositePropertyWidgetQt : public PropertyWidgetQt {

    Q_OBJECT

public:
    CompositePropertyWidgetQt(CompositeProperty* property);
    void updateFromProperty();
    void showWidget();
    void hideWidget();


protected slots:
    void propertyModified();
    void setDisplayModeFromGroupBox();
private:
    CompositeProperty* property_;
    CollapsableGroupBoxWidgetQt* collapsiveGroupBoxWidget_;
    
    void generateWidget();

    std::vector<PropertyWidgetQt*> subPropertyWidgets_;
};

} // namespace

#endif // IVW_COMPOSITEPROPERTYWIDGETQT_H