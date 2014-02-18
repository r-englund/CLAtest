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
 * Main file author: Alexander Johansson
 *
 *********************************************************************************/

#ifndef IVW_FLOATMAT4PROPERTYWIDGETQT_H
#define IVW_FLOATMAT4PROPERTYWIDGETQT_H

#include <QLineEdit>


#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/qt/widgets/editablelabelqt.h>

#include <inviwo/core/properties/ordinalproperty.h>

namespace inviwo {

class IVW_QTWIDGETS_API FloatMat4PropertyWidgetQt : public PropertyWidgetQt {

    Q_OBJECT

public:
    FloatMat4PropertyWidgetQt(FloatMat4Property* property);

    void updateFromProperty();

private:
    FloatMat4Property* property_;
    QLineEdit* mat0x0_;
    QLineEdit* mat0x1_;
    QLineEdit* mat0x2_;
    QLineEdit* mat0x3_;
    QLineEdit* mat1x0_;
    QLineEdit* mat1x1_;
    QLineEdit* mat1x2_;
    QLineEdit* mat1x3_;
    QLineEdit* mat2x0_;
    QLineEdit* mat2x1_;
    QLineEdit* mat2x2_;
    QLineEdit* mat2x3_;
    QLineEdit* mat3x0_;
    QLineEdit* mat3x1_;
    QLineEdit* mat3x2_;
    QLineEdit* mat3x3_;

    EditableLabelQt* label_;

    void generateWidget();

public slots:
    void set0x0Value();
    void set0x1Value();
    void set0x2Value();
    void set0x3Value();
    void set1x0Value();
    void set1x1Value();
    void set1x2Value();
    void set1x3Value();
    void set2x0Value();
    void set2x1Value();
    void set2x2Value();
    void set2x3Value();
    void set3x0Value();
    void set3x1Value();
    void set3x2Value();
    void set3x3Value();

    void setPropertyValue();
    void setPropertyDisplayName();
};

} // namespace

#endif // IVW_INTMAT4PROPERTYWIDGETQT_H