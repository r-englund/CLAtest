 /*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
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
 * Main file authors: Alexander Johansson, Timo Ropinski
 *
 *********************************************************************************/

#ifndef IVW_FLOATVEC3PROPERTYWIDGETQT_H
#define IVW_FLOATVEC3PROPERTYWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/editablelabelqt.h>
#include <inviwo/qt/widgets/floatsliderwidgetqt.h>
#include <inviwo/qt/widgets/properties/propertysettingswidgetqt.h>
#include <QMenu>

#include <inviwo/qt/widgets/properties/propertywidgetqt.h>

#include <inviwo/core/properties/ordinalproperty.h>


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