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

#ifndef IVW_INTVEC3PROPERTYWIDGETQT_H
#define IVW_INTVEC3PROPERTYWIDGETQT_H

#include <QMenu>
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/editablelabelqt.h>
#include <inviwo/qt/widgets/intsliderwidgetqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/propertysettingswidgetqt.h>
#include <inviwo/core/properties/vectorproperties.h>

namespace inviwo {

class IVW_QTWIDGETS_API IntVec3PropertyWidgetQt : public PropertyWidgetQt {

Q_OBJECT

public:
    IntVec3PropertyWidgetQt(IntVec3Property* property);
    virtual ~IntVec3PropertyWidgetQt();

    void updateFromProperty();

private:
    IntVec3Property* property_;
    PropertySettingsWidgetQt* settingsWidget_;
    EditableLabelQt* label_;
    QMenu* settingsMenu_;
    IntSliderWidgetQt* sliderX_;
    IntSliderWidgetQt* sliderY_;
    IntSliderWidgetQt* sliderZ_;
    QLabel* readOnlyLabel_;
    ivec3 valueVecMax_;
    ivec3 valueVecMin_;
    ivec3 valueIncrement_;
    ivec3 valueVec_;

    void generateWidget();
    void generatesSettingsWidget();

public slots:
    void setPropertyValue();
    void setPropertyDisplayName();

    void showContextMenuX(const QPoint& pos);
    void showContextMenuY(const QPoint& pos);
    void showContextMenuZ(const QPoint& pos);
};

} // namespace

#endif // IVW_INTVEC2PROPERTYWIDGETQT_H