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

#ifndef IVW_INTMINMAXPROPERTYWIDGETQT_H
#define IVW_INTMINMAXPROPERTYWIDGETQT_H
//Qt
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/editablelabelqt.h>
#include <inviwo/qt/widgets/rangesliderqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <QSpinBox>
//Core
#include <inviwo/core/properties/vectorproperties.h>

namespace inviwo {

class IVW_QTWIDGETS_API IntMinMaxPropertyWidgetQt : public PropertyWidgetQt {

Q_OBJECT

public:
    IntMinMaxPropertyWidgetQt(IntMinMaxProperty* property);

    void updateFromProperty();

private:
    IntMinMaxProperty* property_;
    ivec2 valueVec_;
    QLabel* readOnlyLabel_;
    RangeSliderQt* slider_;
    QSpinBox* spinBoxMin_;
    QSpinBox* spinBoxMax_;
    EditableLabelQt* label_;

    bool updatingFromProperty_;

    void generateWidget();

protected slots:
    void setPropertyValue();
    void setPropertyDisplayName();

    void updateFromSlider(int valMin, int valMax);
    void updateFromSpinBoxMin(int val);
    void updateFromSpinBoxMax(int val);
};

} // namespace

#endif // IVW_INTMINMAXPROPERTYWIDGETQT_H