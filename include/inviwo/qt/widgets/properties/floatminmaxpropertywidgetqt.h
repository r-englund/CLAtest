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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#ifndef IVW_FLOATMINMAXPROPERTYWIDGETQT_H
#define IVW_FLOATMINMAXPROPERTYWIDGETQT_H
//Qt
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/editablelabelqt.h>
#include <inviwo/qt/widgets/rangesliderqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <QDoubleSpinBox>
//Core
#include <inviwo/core/properties/vectorproperties.h>

namespace inviwo {

class IVW_QTWIDGETS_API FloatMinMaxPropertyWidgetQt : public PropertyWidgetQt {

Q_OBJECT

public:
    FloatMinMaxPropertyWidgetQt(FloatMinMaxProperty* property);

    void updateFromProperty();

private:
    FloatMinMaxProperty* property_;
    vec2 valueVec_;
    QLabel* readOnlyLabel_;
    RangeSliderQt* slider_;
    QDoubleSpinBox* spinBoxMin_;
    QDoubleSpinBox* spinBoxMax_;
    EditableLabelQt* label_;

    bool updatingFromProperty_;

    void generateWidget();

protected slots:
    void setPropertyValue(float, float);
    void setPropertyDisplayName();

    void updateFromSlider(int valMin, int valMax);
    void updateFromSpinBoxMin(double val);
    void updateFromSpinBoxMax(double val);

protected:
    void setSpinBoxDecimals(float increment);

private:
    float maxNumberOfValues_;
    int sliderMin_;
    int sliderMax_;
};

} // namespace

#endif // IVW_INTMINMAXPROPERTYWIDGETQT_H