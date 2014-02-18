/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Main file author: Erik Sundén
 *
 *********************************************************************************/

#ifndef IVW_FLOATMINMAXPROPERTYWIDGETQT_H
#define IVW_FLOATMINMAXPROPERTYWIDGETQT_H
//Qt
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/editablelabelqt.h>
#include <inviwo/qt/widgets/rangesliderqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/qt/widgets/customdoublespinboxqt.h>
//Core
#include <inviwo/core/properties/minmaxproperty.h>

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
    CustomDoubleSpinBoxQt* spinBoxMin_;
    CustomDoubleSpinBoxQt* spinBoxMax_;
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