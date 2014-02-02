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

#ifndef IVW_FLOATVEC4PROPERTYWIDGETQT_H
#define IVW_FLOATVEC4PROPERTYWIDGETQT_H
//Qt
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/editablelabelqt.h>
#include <inviwo/qt/widgets/floatsliderwidgetqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/propertysettingswidgetqt.h>
#include <QMenu>
//Core
#include <inviwo/core/properties/vectorproperties.h>


namespace inviwo {

class IVW_QTWIDGETS_API FloatVec4PropertyWidgetQt : public PropertyWidgetQt {

    Q_OBJECT

public:
    FloatVec4PropertyWidgetQt(FloatVec4Property* property);
    virtual ~FloatVec4PropertyWidgetQt();

    void updateFromProperty();

private:
    FloatVec4Property* property_;
    FloatSliderWidgetQt* sliderX_;
    FloatSliderWidgetQt* sliderY_;
    FloatSliderWidgetQt* sliderZ_;
    FloatSliderWidgetQt* sliderW_;
    QLabel* readOnlyLabel_;
    PropertySettingsWidgetQt* settingsWidget_;
    QMenu* settingsMenu_;
    EditableLabelQt* label_;
    vec4 valueVec_;

    void generateWidget();
    void generatesSettingsWidget();

public slots:
    void setPropertyValue();
    void setPropertyDisplayName();

    void showContextMenuX(const QPoint& pos);
    void showContextMenuY(const QPoint& pos);
    void showContextMenuZ(const QPoint& pos);
    void showContextMenuW(const QPoint& pos);

};

} // namespace

#endif // IVW_FLOATVEC3PROPERTYWIDGETQT_H