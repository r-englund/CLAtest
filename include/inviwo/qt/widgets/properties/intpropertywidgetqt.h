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

#ifndef IVW_INTPROPERTYWIDGETQT_H
#define IVW_INTPROPERTYWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/editablelabelqt.h>
#include <inviwo/qt/widgets/intsliderwidgetqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/propertysettingswidgetqt.h>
#include <inviwo/core/properties/scalarproperties.h>
#include <QMenu>

namespace inviwo {

class IVW_QTWIDGETS_API IntPropertyWidgetQt : public PropertyWidgetQt {

Q_OBJECT

public:
    IntPropertyWidgetQt(IntProperty* property);
    virtual ~IntPropertyWidgetQt();

    void updateFromProperty();

private:
    IntProperty* property_;
    IntSliderWidgetQt* sliderWidget_;
    QLabel* readOnlyLabel_;
    PropertySettingsWidgetQt* settingsWidget_;
    EditableLabelQt* label_;
    QMenu* settingsMenu_;

    void generateWidget();
    void generatesSettingsWidget();

public slots:
    void setPropertyValue();
    void setPropertyDisplayName();

    void showContextMenu(const QPoint& pos);
};

} // namespace

#endif // IVW_INTPROPERTYWIDGETQT_H