/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2014 Inviwo Foundation
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
 * Main file author: Peter Steneteg
 *
 *********************************************************************************/

#include <inviwo/qt/widgets/properties/sphericalfloatvec3propertywidgetqt.h>

#include <QHBoxLayout>
#include <QLabel>


namespace inviwo {

SphericalFloatVec3PropertyWidgetQt::SphericalFloatVec3PropertyWidgetQt(FloatVec3Property* property)
    : property_(property)
    , valueVec3Max_(property->getMaxValue())
    , valueVec3Min_(property->getMinValue())
    , valueIncrement_(property->getIncrement())
    , settingsWidget_(0) {
    PropertyWidgetQt::setProperty(property_);
    PropertyWidgetQt::generateContextMenu();
    generateWidget();
    updateFromProperty();

    if (!property->getReadOnly())
        generatesSettingsWidget();
}

SphericalFloatVec3PropertyWidgetQt::~SphericalFloatVec3PropertyWidgetQt() {
    delete settingsWidget_;
}

void SphericalFloatVec3PropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();

    if (property_->getReadOnly()) {
        hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
        readOnlyLabel_ = new QLabel();
        hLayout->addWidget(readOnlyLabel_);
        setLayout(hLayout);
    } else {
        label_ = new EditableLabelQt(this, property_->getDisplayName(), PropertyWidgetQt::generatePropertyWidgetMenu());
        hLayout->addWidget(label_);
        QWidget* sliderWidget = new QWidget();
        QVBoxLayout* vLayout = new QVBoxLayout();
        sliderWidget->setLayout(vLayout);
        vLayout->setContentsMargins(0, 0, 0, 0);
        vLayout->setSpacing(0);
        sliderR_ = new FloatSliderWidgetQt();
        sliderTheta_ = new FloatSliderWidgetQt();
        sliderPhi_ = new FloatSliderWidgetQt();
        vLayout->addWidget(sliderR_);
        vLayout->addWidget(sliderTheta_);
        vLayout->addWidget(sliderPhi_);
        hLayout->addWidget(sliderWidget);
        setLayout(hLayout);
        QSizePolicy labelPol = label_->sizePolicy();
        labelPol.setHorizontalStretch(1);
        labelPol.setControlType(QSizePolicy::Label);
        label_->setSizePolicy(labelPol);
        QSizePolicy slidersPol = sliderWidget->sizePolicy();
        slidersPol.setHorizontalStretch(3);
        sliderWidget->setSizePolicy(slidersPol);
        connect(label_, SIGNAL(textChanged()), this, SLOT(setPropertyDisplayName()));
        connect(sliderR_, SIGNAL(valueChanged(float)), this, SLOT(setPropertyValue()));
        connect(sliderTheta_, SIGNAL(valueChanged(float)), this, SLOT(setPropertyValue()));
        connect(sliderPhi_, SIGNAL(valueChanged(float)), this, SLOT(setPropertyValue()));
    }
}

void SphericalFloatVec3PropertyWidgetQt::updateFromProperty() {
    if (property_->getReadOnly()) {
        readOnlyLabel_->setText(QString::number(property_->get().x) + ","
                                + QString::number(property_->get().y) + ","
                                + QString::number(property_->get().z));
        readOnlyLabel_->setToolTip("Min: [" + QString::number((property_->getMinValue()).x) +
                                   "," + QString::number((property_->getMinValue()).y) +
                                   "," + QString::number((property_->getMinValue()).z) +
                                   "] Max: [" + QString::number((property_->getMaxValue()).x) +
                                   "," + QString::number((property_->getMaxValue()).y) +
                                   "," + QString::number((property_->getMinValue()).z) + "]");
    } else {
        valueVec3Max_ = property_->getMaxValue();
        valueVec3Min_ = property_->getMinValue();
        valueIncrement_ = property_->getIncrement();
        vec3 initVal = cartesianToSpherical(property_->get());
        sliderR_->initValue(initVal.x);
        sliderTheta_->initValue(initVal.y);
        sliderPhi_->initValue(initVal.z);
        sliderR_->setRange(0, glm::length(valueVec3Max_));
        float pi = static_cast<float>(M_PI);
        sliderTheta_->setRange(-pi, pi);
        sliderPhi_->setRange(-pi / 2.f, pi / 2.f);
        sliderR_->setValue(initVal.x);
        sliderTheta_->setValue(initVal.y);
        sliderPhi_->setValue(initVal.z);
        vec3 inc = cartesianToSpherical(valueIncrement_);
        sliderR_->setIncrement(inc.x);
        sliderTheta_->setIncrement(inc.y);
        sliderPhi_->setIncrement(inc.z);
    }
}


void SphericalFloatVec3PropertyWidgetQt::generatesSettingsWidget() {
    settingsWidget_ = new PropertySettingsWidgetQt(property_);
    settingsMenu_ = PropertyWidgetQt::generatePropertyWidgetMenu();
    settingsMenu_->addAction("Property settings");
    settingsMenu_->addAction("Set as Min");
    settingsMenu_->addAction("Set as Max");
    sliderR_->setContextMenuPolicy(Qt::CustomContextMenu);
    sliderTheta_->setContextMenuPolicy(Qt::CustomContextMenu);
    sliderPhi_->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(sliderR_, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenuX(const QPoint&)));
    connect(sliderTheta_, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenuY(const QPoint&)));
    connect(sliderPhi_, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenuZ(const QPoint&)));
    connect(developerViewModeAction_, SIGNAL(triggered(bool)), this, SLOT(setDeveloperViewMode(bool)));
    connect(applicationViewModeAction_, SIGNAL(triggered(bool)), this, SLOT(setApplicationViewMode(bool)));
    updateContextMenu();
}

void SphericalFloatVec3PropertyWidgetQt::showContextMenuX(const QPoint& pos) {
    PropertyVisibilityMode appVisibilityMode = getApplicationViewMode();

    if (appVisibilityMode == DEVELOPMENT) {
        updateContextMenu();
        QPoint globalPos = sliderR_->mapToGlobal(pos);
        QAction* selecteditem = settingsMenu_->exec(globalPos);

        if (selecteditem && selecteditem->text() == "Property settings") {
            settingsWidget_->reload();
            settingsWidget_->show();
        } else if (selecteditem && selecteditem->text() == "Set as Min") {
            //Set current value of the slider to min value of the property
            valueVec3Min_ = property_->getMinValue();
            valueVec3Min_.x = sliderR_->getValue();
            property_->setMinValue(valueVec3Min_);
            updateFromProperty();
        } else if (selecteditem && selecteditem->text() == "Set as Max") {
            //Set current value of the slider to max value of the property
            valueVec3Max_ = property_->getMaxValue();
            valueVec3Max_.x = sliderR_->getValue();
            property_->setMaxValue(valueVec3Max_);
            updateFromProperty();
        }
    }
}

void SphericalFloatVec3PropertyWidgetQt::showContextMenuY(const QPoint& pos) {
    PropertyVisibilityMode appVisibilityMode = getApplicationViewMode();

    if (appVisibilityMode == DEVELOPMENT) {
        updateContextMenu();
        QPoint globalPos = sliderTheta_->mapToGlobal(pos);
        QAction* selecteditem = settingsMenu_->exec(globalPos);

        if (selecteditem && selecteditem->text() == "Property settings") {
            settingsWidget_->reload();
            settingsWidget_->show();
        } else if (selecteditem && selecteditem->text() == "Set as Min") {
            //Set current value of the slider to min value of the property
            valueVec3Min_ = property_->getMinValue();
            valueVec3Min_.y = sliderTheta_->getValue();
            property_->setMinValue(valueVec3Min_);
            updateFromProperty();
        } else if (selecteditem && selecteditem->text() == "Set as Max") {
            //Set current value of the slider to max value of the property
            valueVec3Max_ = property_->getMaxValue();
            valueVec3Max_.y = sliderTheta_->getValue();
            property_->setMaxValue(valueVec3Max_);
            updateFromProperty();
        }
    }
}

void SphericalFloatVec3PropertyWidgetQt::showContextMenuZ(const QPoint& pos) {
    PropertyVisibilityMode appVisibilityMode = getApplicationViewMode();

    if (appVisibilityMode == DEVELOPMENT) {
        updateContextMenu();
        QPoint globalPos = sliderPhi_->mapToGlobal(pos);
        QAction* selecteditem = settingsMenu_->exec(globalPos);

        if (selecteditem && selecteditem->text() == "Property settings") {
            settingsWidget_->reload();
            settingsWidget_->show();
        } else if (selecteditem && selecteditem->text() == "Set as Min") {
            //Set current value of the slider to min value of the property
            valueVec3Min_ = property_->getMinValue();
            valueVec3Min_.z = sliderPhi_->getValue();
            property_->setMinValue(valueVec3Min_);
            updateFromProperty();
        } else if (selecteditem && selecteditem->text() == "Set as Max") {
            //Set current value of the slider to max value of the property
            valueVec3Max_ = property_->getMaxValue();
            valueVec3Max_.z = sliderPhi_->getValue();
            property_->setMaxValue(valueVec3Max_);
            updateFromProperty();
        }
    }
}

void SphericalFloatVec3PropertyWidgetQt::setPropertyValue() {
    property_->set(sphericalToCartesian(vec3(sliderR_->getValue(),
                                        sliderTheta_->getValue(),
                                        sliderPhi_->getValue())));
    emit modified();
}

void SphericalFloatVec3PropertyWidgetQt::setPropertyDisplayName() {
    property_->setDisplayName(label_->getText());
}

vec3 SphericalFloatVec3PropertyWidgetQt::cartesianToSpherical(vec3 v) {
    float r = glm::length(v);
    float theta = std::acos(v.z / r);
    float pi = static_cast<float>(M_PI);

    while (theta < -pi)
        theta += 2.f * pi;

    while (theta > pi)
        theta -= 2.f * pi;

    float phi = std::atan(v.y / v.x);

    while (phi < -pi / 2.f)
        phi += pi;

    while (phi > pi / 2.f)
        phi -= pi;

    return vec3(r, std::acos(v.z / r), std::atan(v.y / v.x));
}

vec3 SphericalFloatVec3PropertyWidgetQt::sphericalToCartesian(vec3 v) {
    return vec3(v.x * std::sin(v.y) * std::cos(v.z),
                v.x * std::sin(v.y) * std::sin(v.z),
                v.x * std::cos(v.y));
}


} // namespace

