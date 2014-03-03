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

#include <inviwo/qt/widgets/properties/floatmat3propertywidgetqt.h>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>

namespace inviwo {

FloatMat3PropertyWidgetQt::FloatMat3PropertyWidgetQt(FloatMat3Property* property) : PropertyWidgetQt(property), property_(property) {
    generateWidget();
    updateFromProperty();
}

void FloatMat3PropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    QGroupBox* matrixgroup = new QGroupBox();
    mat0x0_ = new QLineEdit;
    mat0x1_ = new QLineEdit;
    mat0x2_ = new QLineEdit;
    mat1x0_ = new QLineEdit;
    mat1x1_ = new QLineEdit;
    mat1x2_ = new QLineEdit;
    mat2x0_ = new QLineEdit;
    mat2x1_ = new QLineEdit;
    mat2x2_ = new QLineEdit;

    if (property_->getReadOnly()) {
        hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
        mat0x0_->setDisabled(true);
        mat0x1_->setDisabled(true);
        mat0x2_->setDisabled(true);
        mat1x0_->setDisabled(true);
        mat1x1_->setDisabled(true);
        mat1x2_->setDisabled(true);
        mat2x0_->setDisabled(true);
        mat2x1_->setDisabled(true);
        mat2x2_->setDisabled(true);
    }
    else {
        connect(mat0x0_, SIGNAL(editingFinished()),
                this, SLOT(set0x0Value()));
        connect(mat0x1_, SIGNAL(editingFinished()),
                this, SLOT(set0x1Value()));
        connect(mat0x2_, SIGNAL(editingFinished()),
                this, SLOT(set0x2Value()));
        connect(mat1x0_, SIGNAL(editingFinished()),
                this, SLOT(set1x0Value()));
        connect(mat1x1_, SIGNAL(editingFinished()),
                this, SLOT(set1x1Value()));
        connect(mat1x2_, SIGNAL(editingFinished()),
                this, SLOT(set1x2Value()));
        connect(mat2x0_, SIGNAL(editingFinished()),
                this, SLOT(set2x0Value()));
        connect(mat2x1_, SIGNAL(editingFinished()),
                this, SLOT(set2x1Value()));
        connect(mat2x2_, SIGNAL(editingFinished()),
                this, SLOT(set2x2Value()));
        label_ = new EditableLabelQt(this,property_->getDisplayName(),PropertyWidgetQt::getContextMenu());
        hLayout->addWidget(label_);
        connect(label_, SIGNAL(textChanged()),this, SLOT(setPropertyDisplayName()));
    }

    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->addWidget(mat0x0_,0,0);
    gridLayout->addWidget(mat0x1_,0,1);
    gridLayout->addWidget(mat0x2_,0,2);
    gridLayout->addWidget(mat1x0_,1,0);
    gridLayout->addWidget(mat1x1_,1,1);
    gridLayout->addWidget(mat1x2_,1,2);
    gridLayout->addWidget(mat2x0_,2,0);
    gridLayout->addWidget(mat2x1_,2,1);
    gridLayout->addWidget(mat2x2_,2,2);
    matrixgroup->setLayout(gridLayout);
    hLayout->addWidget(matrixgroup);
    setLayout(hLayout);
}

void FloatMat3PropertyWidgetQt::set0x0Value() {
    float valuef =(mat0x0_->text()).toFloat();
    mat3 valueMat3 =property_->get();
    valueMat3[0][0] = valuef;
    property_->set(valueMat3);
    emit modified();
}

void FloatMat3PropertyWidgetQt::set0x1Value() {
    float valuef =(mat0x1_->text()).toFloat();
    mat3 valueMat3 =property_->get();
    valueMat3[0][1] = valuef;
    property_->set(valueMat3);
    emit modified();
}

void FloatMat3PropertyWidgetQt::set0x2Value() {
    float valuef =(mat0x2_->text()).toFloat();
    mat3 valueMat3 =property_->get();
    valueMat3[0][2] = valuef;
    property_->set(valueMat3);
    emit modified();
}

void FloatMat3PropertyWidgetQt::set1x0Value() {
    float valuef =(mat1x0_->text()).toFloat();
    mat3 valueMat3 =property_->get();
    valueMat3[1][0] = valuef;
    property_->set(valueMat3);
    emit modified();
}

void FloatMat3PropertyWidgetQt::set1x1Value() {
    float valuef =(mat1x1_->text()).toFloat();
    mat3 valueMat3 =property_->get();
    valueMat3[1][1] = valuef;
    property_->set(valueMat3);
    emit modified();
}

void FloatMat3PropertyWidgetQt::set1x2Value() {
    float valuef =(mat1x0_->text()).toFloat();
    mat3 valueMat3 =property_->get();
    valueMat3[1][2] = valuef;
    property_->set(valueMat3);
    emit modified();
}

void FloatMat3PropertyWidgetQt::set2x0Value() {
    float valuef =(mat2x0_->text()).toFloat();
    mat3 valueMat3 =property_->get();
    valueMat3[2][0] = valuef;
    property_->set(valueMat3);
    emit modified();
}

void FloatMat3PropertyWidgetQt::set2x1Value() {
    float valuef =(mat2x1_->text()).toFloat();
    mat3 valueMat3 =property_->get();
    valueMat3[2][1] = valuef;
    property_->set(valueMat3);
    emit modified();
}

void FloatMat3PropertyWidgetQt::set2x2Value() {
    float valuef =(mat2x2_->text()).toFloat();
    mat3 valueMat3 =property_->get();
    valueMat3[2][2] = valuef;
    property_->set(valueMat3);
    emit modified();
}

void FloatMat3PropertyWidgetQt::setPropertyValue() {
    set0x0Value();
    set0x1Value();
    set0x2Value();
    set1x0Value();
    set1x1Value();
    set1x2Value();
    set2x0Value();
    set2x1Value();
    set2x2Value();
}

void FloatMat3PropertyWidgetQt::updateFromProperty() {
    mat3 value =property_->get();
    mat0x0_->setText(QString::number(value[0][0]));
    mat0x1_->setText(QString::number(value[0][1]));
    mat0x2_->setText(QString::number(value[0][2]));
    mat1x0_->setText(QString::number(value[1][0]));
    mat1x1_->setText(QString::number(value[1][1]));
    mat1x2_->setText(QString::number(value[1][2]));
    mat2x0_->setText(QString::number(value[2][0]));
    mat2x1_->setText(QString::number(value[2][1]));
    mat2x2_->setText(QString::number(value[2][2]));
};

void FloatMat3PropertyWidgetQt::setPropertyDisplayName() {
    property_->setDisplayName(label_->getText());
}


} //namespace