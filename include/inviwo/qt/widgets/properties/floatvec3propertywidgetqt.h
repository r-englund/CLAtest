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
 * Primary author : Alexander Johansson
 *
 **********************************************************************/

#ifndef IVW_FLOATVEC3PROPERTYWIDGETQT_H
#define IVW_FLOATVEC3PROPERTYWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/editablelabelqt.h>
#include <inviwo/qt/widgets/floatsliderwidgetqt.h>
#include <inviwo/qt/widgets/properties/propertysettingswidgetqt.h>
#include <QMenu>

#include <inviwo/qt/widgets/properties/propertywidgetqt.h>

#include <inviwo/core/properties/vectorproperties.h>


namespace inviwo {

class IVW_QTWIDGETS_API FloatVec3PropertyWidgetQt : public PropertyWidgetQt {

    Q_OBJECT

public:
    FloatVec3PropertyWidgetQt(FloatVec3Property* property);
    virtual ~FloatVec3PropertyWidgetQt();

    void updateFromProperty();

private:
    FloatVec3Property* property_;
    FloatSliderWidgetQt* sliderX_;
    FloatSliderWidgetQt* sliderY_;
    FloatSliderWidgetQt* sliderZ_;
    QLabel* readOnlyLabel_;
    PropertySettingsWidgetQt* settingsWidget_;
    vec3 valueVec3Max_;
    vec3 valueVec3Min_;
    vec3 valueIncrement_;
    QMenu* settingsMenu_;
    EditableLabelQt* label_;

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

#endif // IVW_FLOATVEC3PROPERTYWIDGETQT_H