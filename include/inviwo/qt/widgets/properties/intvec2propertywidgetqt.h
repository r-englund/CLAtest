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
 * Primary author : Alexander Johansson
 *
 **********************************************************************/

#ifndef IVW_INTVEC2PROPERTYWIDGETQT_H
#define IVW_INTVEC2PROPERTYWIDGETQT_H

#include <QMenu>
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/editablelabelqt.h>
#include <inviwo/qt/widgets/intsliderwidgetqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/propertysettingswidgetqt.h>
#include <inviwo/core/properties/vectorproperties.h>

namespace inviwo {

class IVW_QTWIDGETS_API IntVec2PropertyWidgetQt : public PropertyWidgetQt {

Q_OBJECT

public:
    IntVec2PropertyWidgetQt(IntVec2Property* property);

    void updateFromProperty();

private:
    IntVec2Property* property_;
    PropertySettingsWidgetQt* settingsWidget_;
    EditableLabelQt* label_;
    QMenu* settingsMenu_;
    QLabel* readOnlyLabel_;
    IntSliderWidgetQt* sliderX_;
    IntSliderWidgetQt* sliderY_;
    ivec2 valueVecMax_;
    ivec2 valueVecMin_;
    ivec2 valueIncrement_;
    ivec2 valueVec_;

    void generateWidget();
    void generatesSettingsWidget();

public slots:
    void setPropertyValue();
    void setPropertyDisplayName();

    void showContextMenuX(const QPoint& pos);
    void showContextMenuY(const QPoint& pos);

};

} // namespace

#endif // IVW_INTVEC2PROPERTYWIDGETQT_H