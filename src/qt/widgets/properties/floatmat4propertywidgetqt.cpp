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

#include <inviwo/qt/widgets/properties/floatmat4propertywidgetqt.h>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>

namespace inviwo {

FloatMat4PropertyWidgetQt::FloatMat4PropertyWidgetQt(FloatMat4Property* property) : property_(property) {
    PropertyWidgetQt::setProperty(property_);
    PropertyWidgetQt::generateContextMenu();
    generateWidget();
    updateFromProperty();
}

void FloatMat4PropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    QGroupBox* matrixgroup = new QGroupBox();
    mat0x0_ = new QLineEdit;
    mat0x1_ = new QLineEdit;
    mat0x2_ = new QLineEdit;
    mat0x3_ = new QLineEdit;
    mat1x0_ = new QLineEdit;
    mat1x1_ = new QLineEdit;
    mat1x2_ = new QLineEdit;
    mat1x3_ = new QLineEdit;
    mat2x0_ = new QLineEdit;
    mat2x1_ = new QLineEdit;
    mat2x2_ = new QLineEdit;
    mat2x3_ = new QLineEdit;
    mat3x0_ = new QLineEdit;
    mat3x1_ = new QLineEdit;
    mat3x2_ = new QLineEdit;
    mat3x3_ = new QLineEdit;

    if (property_->getReadOnly()) {
        hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
        mat0x0_->setDisabled(true);
        mat0x1_->setDisabled(true);
        mat0x2_->setDisabled(true);
        mat0x3_->setDisabled(true);
        mat1x0_->setDisabled(true);
        mat1x1_->setDisabled(true);
        mat1x2_->setDisabled(true);
        mat1x3_->setDisabled(true);
        mat2x0_->setDisabled(true);
        mat2x1_->setDisabled(true);
        mat2x2_->setDisabled(true);
        mat2x3_->setDisabled(true);
        mat3x0_->setDisabled(true);
        mat3x1_->setDisabled(true);
        mat3x2_->setDisabled(true);
        mat3x3_->setDisabled(true);
    }
    else
    {
        connect(mat0x0_, SIGNAL(editingFinished()),
                this, SLOT(set0x0Value()));
        connect(mat0x1_, SIGNAL(editingFinished()),
                this, SLOT(set0x1Value()));
        connect(mat0x2_, SIGNAL(editingFinished()),
                this, SLOT(set0x2Value()));
        connect(mat0x3_, SIGNAL(editingFinished()),
                this, SLOT(set0x3Value()));
        connect(mat1x0_, SIGNAL(editingFinished()),
                this, SLOT(set1x0Value()));
        connect(mat1x1_, SIGNAL(editingFinished()),
                this, SLOT(set1x1Value()));
        connect(mat1x2_, SIGNAL(editingFinished()),
                this, SLOT(set1x2Value()));
        connect(mat1x3_, SIGNAL(editingFinished()),
                this, SLOT(set1x3Value()));
        connect(mat2x0_, SIGNAL(editingFinished()),
                this, SLOT(set2x0Value()));
        connect(mat2x1_, SIGNAL(editingFinished()),
                this, SLOT(set2x1Value()));
        connect(mat2x2_, SIGNAL(editingFinished()),
                this, SLOT(set2x2Value()));
        connect(mat2x3_, SIGNAL(editingFinished()),
                this, SLOT(set2x3Value()));
        connect(mat3x0_, SIGNAL(editingFinished()),
                this, SLOT(set3x0Value()));
        connect(mat3x1_, SIGNAL(editingFinished()),
                this, SLOT(set3x1Value()));
        connect(mat3x2_, SIGNAL(editingFinished()),
                this, SLOT(set3x2Value()));
        connect(mat3x3_, SIGNAL(editingFinished()),
                this, SLOT(set3x3Value()));
        label_ = new EditableLabelQt(this,property_->getDisplayName(),PropertyWidgetQt::generatePropertyWidgetMenu());
        hLayout->addWidget(label_);
        connect(label_, SIGNAL(textChanged()),this, SLOT(setPropertyDisplayName()));
    }

    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->addWidget(mat0x0_,0,0);
    gridLayout->addWidget(mat0x1_,0,1);
    gridLayout->addWidget(mat0x2_,0,2);
    gridLayout->addWidget(mat0x3_,0,3);
    gridLayout->addWidget(mat1x0_,1,0);
    gridLayout->addWidget(mat1x1_,1,1);
    gridLayout->addWidget(mat1x2_,1,2);
    gridLayout->addWidget(mat1x3_,1,3);
    gridLayout->addWidget(mat2x0_,2,0);
    gridLayout->addWidget(mat2x1_,2,1);
    gridLayout->addWidget(mat2x2_,2,2);
    gridLayout->addWidget(mat2x3_,2,3);
    gridLayout->addWidget(mat3x0_,3,0);
    gridLayout->addWidget(mat3x1_,3,1);
    gridLayout->addWidget(mat3x2_,3,2);
    gridLayout->addWidget(mat3x3_,3,3);
    matrixgroup->setLayout(gridLayout);
    hLayout->addWidget(matrixgroup);
    setLayout(hLayout);
}

void FloatMat4PropertyWidgetQt::set0x0Value() {
    float valuef =(mat0x0_->text()).toFloat();
    mat4 valueMat4 =property_->get();
    valueMat4[0][0] = valuef;
    property_->set(valueMat4);
    emit modified();
}

void FloatMat4PropertyWidgetQt::set0x1Value() {
    float valuef =(mat0x1_->text()).toFloat();
    mat4 valueMat4 =property_->get();
    valueMat4[0][1] = valuef;
    property_->set(valueMat4);
    emit modified();
}

void FloatMat4PropertyWidgetQt::set0x2Value() {
    float valuef =(mat0x2_->text()).toFloat();
    mat4 valueMat4 =property_->get();
    valueMat4[0][2] = valuef;
    property_->set(valueMat4);
    emit modified();
}

void FloatMat4PropertyWidgetQt::set0x3Value() {
    float valuef =(mat0x3_->text()).toFloat();
    mat4 valueMat4 =property_->get();
    valueMat4[0][3] = valuef;
    property_->set(valueMat4);
    emit modified();
}

void FloatMat4PropertyWidgetQt::set1x0Value() {
    float valuef =(mat1x0_->text()).toFloat();
    mat4 valueMat4 =property_->get();
    valueMat4[1][0] = valuef;
    property_->set(valueMat4);
    emit modified();
}

void FloatMat4PropertyWidgetQt::set1x1Value() {
    float valuef =(mat1x1_->text()).toFloat();
    mat4 valueMat4 =property_->get();
    valueMat4[1][1] = valuef;
    property_->set(valueMat4);
    emit modified();
}

void FloatMat4PropertyWidgetQt::set1x2Value() {
    float valuef =(mat1x0_->text()).toFloat();
    mat4 valueMat4 =property_->get();
    valueMat4[1][2] = valuef;
    property_->set(valueMat4);
    emit modified();
}

void FloatMat4PropertyWidgetQt::set1x3Value() {
    float valuef =(mat1x3_->text()).toFloat();
    mat4 valueMat4 =property_->get();
    valueMat4[1][3] = valuef;
    property_->set(valueMat4);
    emit modified();
}

void FloatMat4PropertyWidgetQt::set2x0Value() {
    float valuef =(mat2x0_->text()).toFloat();
    mat4 valueMat4 =property_->get();
    valueMat4[2][0] = valuef;
    property_->set(valueMat4);
    emit modified();
}

void FloatMat4PropertyWidgetQt::set2x1Value() {
    float valuef =(mat2x1_->text()).toFloat();
    mat4 valueMat4 =property_->get();
    valueMat4[2][1] = valuef;
    property_->set(valueMat4);
    emit modified();
}

void FloatMat4PropertyWidgetQt::set2x2Value() {
    float valuef =(mat2x2_->text()).toFloat();
    mat4 valueMat4 =property_->get();
    valueMat4[2][2] = valuef;
    property_->set(valueMat4);
    emit modified();
}

void FloatMat4PropertyWidgetQt::set2x3Value() {
    float valuef =(mat2x3_->text()).toFloat();
    mat4 valueMat4 =property_->get();
    valueMat4[2][3] = valuef;
    property_->set(valueMat4);
    emit modified();
}

void FloatMat4PropertyWidgetQt::set3x0Value() {
    float valuef =(mat3x0_->text()).toFloat();
    mat4 valueMat4 =property_->get();
    valueMat4[3][0] = valuef;
    property_->set(valueMat4);
    emit modified();
}

void FloatMat4PropertyWidgetQt::set3x1Value() {
    float valuef =(mat3x1_->text()).toFloat();
    mat4 valueMat4 =property_->get();
    valueMat4[3][1] = valuef;
    property_->set(valueMat4);
    emit modified();
}

void FloatMat4PropertyWidgetQt::set3x2Value() {
    float valuef =(mat3x2_->text()).toFloat();
    mat4 valueMat4 =property_->get();
    valueMat4[3][2] = valuef;
    property_->set(valueMat4);
    emit modified();
}

void FloatMat4PropertyWidgetQt::set3x3Value() {
    float valuef =(mat3x3_->text()).toFloat();
    mat4 valueMat4 =property_->get();
    valueMat4[3][3] = valuef;
    property_->set(valueMat4);
    emit modified();
}

void FloatMat4PropertyWidgetQt::setPropertyValue() {
    set0x0Value();
    set0x1Value();
    set0x2Value();
    set0x3Value();
    set1x0Value();
    set1x1Value();
    set1x2Value();
    set1x3Value();
    set2x0Value();
    set2x1Value();
    set2x2Value();
    set2x3Value();
    set3x0Value();
    set3x1Value();
    set3x2Value();
    set3x3Value();
}

void FloatMat4PropertyWidgetQt::updateFromProperty() {
    mat4 value =property_->get();
    mat0x0_->setText(QString::number(value[0][0]));
    mat0x1_->setText(QString::number(value[0][1]));
    mat0x2_->setText(QString::number(value[0][2]));
    mat0x3_->setText(QString::number(value[0][3]));
    mat1x0_->setText(QString::number(value[1][0]));
    mat1x1_->setText(QString::number(value[1][1]));
    mat1x2_->setText(QString::number(value[1][2]));
    mat1x3_->setText(QString::number(value[1][3]));
    mat2x0_->setText(QString::number(value[2][0]));
    mat2x1_->setText(QString::number(value[2][1]));
    mat2x2_->setText(QString::number(value[2][2]));
    mat2x3_->setText(QString::number(value[2][3]));
    mat3x0_->setText(QString::number(value[3][0]));
    mat3x1_->setText(QString::number(value[3][1]));
    mat3x2_->setText(QString::number(value[3][2]));
    mat3x3_->setText(QString::number(value[3][3]));
};

void FloatMat4PropertyWidgetQt::setPropertyDisplayName() {
    property_->setDisplayName(label_->getText());
}


} //namespace