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
 * Main file authors: Alexander Johansson, Rickard Englund
 *
 *********************************************************************************/

#include <inviwo/qt/widgets/properties/propertysettingswidgetqt.h>

namespace inviwo {

PropertySettingsWidgetQt::PropertySettingsWidgetQt(Property* property)
    : QWidget(0)
    , property_(property)
    , floatProperty_(NULL)
    , intProperty_(NULL)
    , floatVec2Property_(NULL)
    , floatVec3Property_(NULL)
    , floatVec4Property_(NULL)
    , intVec2Property_(NULL)
    , intVec3Property_(NULL)
    , intVec4Property_(NULL)

    , lineEditMaxX_(NULL)
    , lineEditMaxY_(NULL)
    , lineEditMaxZ_(NULL)
    , lineEditMaxW_(NULL)

    , lineEditMinX_(NULL)
    , lineEditMinY_(NULL)
    , lineEditMinZ_(NULL)
    , lineEditMinW_(NULL)

    , lineEditIcrementX_(NULL)
    , lineEditIcrementY_(NULL)
    , lineEditIcrementZ_(NULL)
    , lineEditIcrementW_(NULL)

    , xValueLabel_(NULL)
    , yValueLabel_(NULL)
    , zValueLabel_(NULL)
    , wValueLabel_(NULL)

    , minValueLabel_(NULL)
    , maxValueLabel_(NULL)
    , incValueLabel_(NULL) {
    if (dynamic_cast<FloatProperty*>(property_))
        floatProperty_ = static_cast<FloatProperty*>(property_);

    if (dynamic_cast<FloatVec2Property*>(property_))
        floatVec2Property_ = static_cast<FloatVec2Property*>(property_);

    if (dynamic_cast<FloatVec3Property*>(property_))
        floatVec3Property_ = static_cast<FloatVec3Property*>(property_);

    if (dynamic_cast<FloatVec4Property*>(property_))
        floatVec4Property_ = static_cast<FloatVec4Property*>(property_);

    if (dynamic_cast<IntProperty*>(property_))
        intProperty_ = static_cast<IntProperty*>(property_);

    if (dynamic_cast<IntVec2Property*>(property_))
        intVec2Property_ = static_cast<IntVec2Property*>(property_);

    if (dynamic_cast<IntVec3Property*>(property_))
        intVec3Property_ = static_cast<IntVec3Property*>(property_);

    if (dynamic_cast<IntVec4Property*>(property_))
        intVec4Property_ = static_cast<IntVec4Property*>(property_);

    generateWidget();
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->setWindowModality(Qt::ApplicationModal);
    this->setFixedSize(300,125);
}

#define DELETE_LATER(prop) if(prop) delete prop;
PropertySettingsWidgetQt::~PropertySettingsWidgetQt() {
    DELETE_LATER(lineEditMaxX_);
    DELETE_LATER(lineEditMaxY_);
    DELETE_LATER(lineEditMaxZ_);
    DELETE_LATER(lineEditMaxW_);
    DELETE_LATER(lineEditMinX_);
    DELETE_LATER(lineEditMinY_);
    DELETE_LATER(lineEditMinZ_);
    DELETE_LATER(lineEditMinW_);
    DELETE_LATER(lineEditIcrementX_);
    DELETE_LATER(lineEditIcrementY_);
    DELETE_LATER(lineEditIcrementZ_);
    DELETE_LATER(lineEditIcrementW_);;
    DELETE_LATER(xValueLabel_);
    DELETE_LATER(yValueLabel_);
    DELETE_LATER(zValueLabel_);
    DELETE_LATER(wValueLabel_);
    DELETE_LATER(minValueLabel_);
    DELETE_LATER(maxValueLabel_);
    DELETE_LATER(incValueLabel_);
    DELETE_LATER(gridLayout_);
    DELETE_LATER(btnApply_);
    DELETE_LATER(btnCancel_);
}

#define FIX_TAB_ORDER(e) if(e){order.push_back(e);}
void PropertySettingsWidgetQt::fixTabOrder(){
    std::vector<QWidget*> order;
    FIX_TAB_ORDER(lineEditMinX_);
    FIX_TAB_ORDER(lineEditMinY_);
    FIX_TAB_ORDER(lineEditMinZ_);
    FIX_TAB_ORDER(lineEditMinW_);
    FIX_TAB_ORDER(lineEditMaxX_);
    FIX_TAB_ORDER(lineEditMaxY_);
    FIX_TAB_ORDER(lineEditMaxZ_);
    FIX_TAB_ORDER(lineEditMaxW_);
    FIX_TAB_ORDER(lineEditIcrementX_);
    FIX_TAB_ORDER(lineEditIcrementY_);
    FIX_TAB_ORDER(lineEditIcrementZ_);
    FIX_TAB_ORDER(lineEditIcrementW_);
    FIX_TAB_ORDER(lineEditIcrementW_);
    FIX_TAB_ORDER(btnApply_);
    FIX_TAB_ORDER(btnCancel_);

    for(size_t i = 0;i<order.size()-1;i++){
        setTabOrder(order[i],order[i+1]);
    }

    setTabOrder(order.back(),order[0]);
}

void PropertySettingsWidgetQt::generateWidget() {
    btnApply_ = new QPushButton("Apply",this);
    btnCancel_ = new QPushButton("Cancel",this);
    connect(btnApply_,SIGNAL(clicked()),this,SLOT(save()));
    connect(btnCancel_,SIGNAL(clicked()),this,SLOT(cancel()));
    gridLayout_ = new QGridLayout(this);

    minValueLabel_ = new QLabel("Min value",this);
    maxValueLabel_ = new QLabel("Max value",this);
    incValueLabel_ = new QLabel("Increment",this);


    gridLayout_->addWidget(minValueLabel_,1,1);
    gridLayout_->addWidget(maxValueLabel_,2,1);
    gridLayout_->addWidget(incValueLabel_,3,1);



    if (floatProperty_!= NULL || intProperty_!= NULL){
        generateSingleWidget();
        gridLayout_->addWidget(btnApply_,4,1);
        gridLayout_->addWidget(btnCancel_,4,2);
    }

    if (floatVec2Property_ != NULL || intVec2Property_ != NULL){
        generateVec2Widget();
        gridLayout_->addWidget(btnApply_,5,1,1,2);
        gridLayout_->addWidget(btnCancel_,5,3,1,2);
    }

    if (floatVec3Property_ != NULL || intVec3Property_ != NULL){
        generateVec3Widget();
        gridLayout_->addWidget(btnApply_,5,2,1,2);
        gridLayout_->addWidget(btnCancel_,5,4,1,1);
    }

    if (floatVec4Property_!=NULL || intVec4Property_!=NULL){
        generateVec4Widget();
        gridLayout_->addWidget(btnApply_,5,2,1,2);
        gridLayout_->addWidget(btnCancel_,5,4,1,2);
    }


    setLayout(gridLayout_);

    fixTabOrder();

    reload();
    setWindowTitle(QString::fromStdString(property_->getDisplayName()));
}

void PropertySettingsWidgetQt::generateSingleWidget(){
    xValueLabel_ = new QLabel("X",this);
    lineEditMinX_ = new QLineEdit(this);
    lineEditMaxX_ = new QLineEdit(this);
    lineEditIcrementX_ = new QLineEdit(this);

    
    gridLayout_->addWidget(xValueLabel_      ,0,2,1,1,Qt::AlignHCenter);
    gridLayout_->addWidget(lineEditMinX_     ,1,2);
    gridLayout_->addWidget(lineEditMaxX_     ,2,2);
    gridLayout_->addWidget(lineEditIcrementX_,3,2);
    gridLayout_->setColumnStretch(2,2);
}

void PropertySettingsWidgetQt::generateVec2Widget() {
    generateSingleWidget();
    yValueLabel_ = new QLabel("Y",this);
    lineEditMinY_ = new QLineEdit(this);
    lineEditMaxY_ = new QLineEdit(this);
    lineEditIcrementY_ = new QLineEdit(this);


    gridLayout_->addWidget(yValueLabel_      ,0,3,1,1,Qt::AlignHCenter);
    gridLayout_->addWidget(lineEditMinY_     ,1,3);
    gridLayout_->addWidget(lineEditMaxY_     ,2,3);
    gridLayout_->addWidget(lineEditIcrementY_,3,3);
    gridLayout_->setColumnStretch(3,2);
}

void PropertySettingsWidgetQt::generateVec3Widget() {
    generateVec2Widget();
    zValueLabel_ = new QLabel("Z",this);
    lineEditMinZ_ = new QLineEdit(this);
    lineEditMaxZ_ = new QLineEdit(this);
    lineEditIcrementZ_ = new QLineEdit(this);


    gridLayout_->addWidget(zValueLabel_      ,0,4,1,1,Qt::AlignHCenter);
    gridLayout_->addWidget(lineEditMinZ_     ,1,4);
    gridLayout_->addWidget(lineEditMaxZ_     ,2,4);
    gridLayout_->addWidget(lineEditIcrementZ_,3,4);
    gridLayout_->setColumnStretch(4,2);
}

void PropertySettingsWidgetQt::generateVec4Widget() {
    generateVec3Widget();
    wValueLabel_ = new QLabel("W",this);
    lineEditMinW_ = new QLineEdit(this);
    lineEditMaxW_ = new QLineEdit(this);
    lineEditIcrementW_ = new QLineEdit(this);


    gridLayout_->addWidget(wValueLabel_      ,0,5,1,1,Qt::AlignHCenter);
    gridLayout_->addWidget(lineEditMinW_     ,1,5);
    gridLayout_->addWidget(lineEditMaxW_     ,2,5);
    gridLayout_->addWidget(lineEditIcrementW_,3,5);
    gridLayout_->setColumnStretch(5,2);
}

void PropertySettingsWidgetQt::cancel() {
    hide();
    reload();
}

void PropertySettingsWidgetQt::reload() {
    if (floatProperty_!=NULL) {
        lineEditMinX_->setText(QString::number(floatProperty_->getMinValue()));
        lineEditMaxX_->setText(QString::number(floatProperty_->getMaxValue()));
        lineEditIcrementX_->setText(QString::number(floatProperty_->getIncrement()));
        return;
    }

    if (intProperty_!=NULL) {
        lineEditMinX_->setText(QString::number(intProperty_->getMinValue()));
        lineEditMaxX_->setText(QString::number(intProperty_->getMaxValue()));
        lineEditIcrementX_->setText(QString::number(intProperty_->getIncrement()));
        return;
    }

    if (floatVec2Property_!=NULL) {
        vec2 vec2MinValue = floatVec2Property_->getMinValue();
        vec2 vec2MaxValue = floatVec2Property_->getMaxValue();
        vec2 vec2Increment = floatVec2Property_->getIncrement();
        lineEditMinX_->setText(QString::number(vec2MinValue.x));
        lineEditMinY_->setText(QString::number(vec2MinValue.y));
        lineEditMaxX_->setText(QString::number(vec2MaxValue.x));
        lineEditMaxY_->setText(QString::number(vec2MaxValue.y));
        lineEditIcrementX_->setText(QString::number(vec2Increment.x));
        lineEditIcrementY_->setText(QString::number(vec2Increment.y));
        return;
    }

    if (floatVec3Property_!=NULL) {
        vec3 vec3MinValue = floatVec3Property_->getMinValue();
        vec3 vec3MaxValue = floatVec3Property_->getMaxValue();
        vec3 vec3Increment = floatVec3Property_->getIncrement();
        lineEditMinX_->setText(QString::number(vec3MinValue.x));
        lineEditMinY_->setText(QString::number(vec3MinValue.y));
        lineEditMinZ_->setText(QString::number(vec3MinValue.z));
        lineEditMaxX_->setText(QString::number(vec3MaxValue.x));
        lineEditMaxY_->setText(QString::number(vec3MaxValue.y));
        lineEditMaxZ_->setText(QString::number(vec3MaxValue.z));
        lineEditIcrementX_->setText(QString::number(vec3Increment.x));
        lineEditIcrementY_->setText(QString::number(vec3Increment.y));
        lineEditIcrementZ_->setText(QString::number(vec3Increment.z));
        return;
    }

    if (floatVec4Property_!=NULL) {
        vec4 vec4MinValue = floatVec4Property_->getMinValue();
        vec4 vec4MaxValue = floatVec4Property_->getMaxValue();
        vec4 vec4Increment = floatVec4Property_->getIncrement();
        lineEditMinX_->setText(QString::number(vec4MinValue.x));
        lineEditMinY_->setText(QString::number(vec4MinValue.y));
        lineEditMinZ_->setText(QString::number(vec4MinValue.z));
        lineEditMinW_->setText(QString::number(vec4MinValue.w));
        lineEditMaxX_->setText(QString::number(vec4MaxValue.x));
        lineEditMaxY_->setText(QString::number(vec4MaxValue.y));
        lineEditMaxZ_->setText(QString::number(vec4MaxValue.z));
        lineEditMaxW_->setText(QString::number(vec4MaxValue.w));
        lineEditIcrementX_->setText(QString::number(vec4Increment.x));
        lineEditIcrementY_->setText(QString::number(vec4Increment.y));
        lineEditIcrementZ_->setText(QString::number(vec4Increment.z));
        lineEditIcrementW_->setText(QString::number(vec4Increment.w));
        return;
    }

    if (intVec2Property_!=NULL) {
        ivec2 vec2MinValue = intVec2Property_->getMinValue();
        ivec2 vec2MaxValue = intVec2Property_->getMaxValue();
        ivec2 vec2Increment = intVec2Property_->getIncrement();
        lineEditMinX_->setText(QString::number(vec2MinValue.x));
        lineEditMinY_->setText(QString::number(vec2MinValue.y));
        lineEditMaxX_->setText(QString::number(vec2MaxValue.x));
        lineEditMaxY_->setText(QString::number(vec2MaxValue.y));
        lineEditIcrementX_->setText(QString::number(vec2Increment.x));
        lineEditIcrementY_->setText(QString::number(vec2Increment.y));
        return;
    }

    if (intVec3Property_!=NULL) {
        ivec3 vec3MinValue = intVec3Property_->getMinValue();
        ivec3 vec3MaxValue = intVec3Property_->getMaxValue();
        ivec3 vec3Increment = intVec3Property_->getIncrement();
        lineEditMinX_->setText(QString::number(vec3MinValue.x));
        lineEditMinY_->setText(QString::number(vec3MinValue.y));
        lineEditMinZ_->setText(QString::number(vec3MinValue.z));
        lineEditMaxX_->setText(QString::number(vec3MaxValue.x));
        lineEditMaxY_->setText(QString::number(vec3MaxValue.y));
        lineEditMaxZ_->setText(QString::number(vec3MaxValue.z));
        lineEditIcrementX_->setText(QString::number(vec3Increment.x));
        lineEditIcrementY_->setText(QString::number(vec3Increment.y));
        lineEditIcrementZ_->setText(QString::number(vec3Increment.z));
        return;
    }

    if (intVec4Property_ != NULL) {
        ivec4 vec4MinValue = intVec4Property_->getMinValue();
        ivec4 vec4MaxValue = intVec4Property_->getMaxValue();
        ivec4 vec4Increment = intVec4Property_->getIncrement();
        lineEditMinX_->setText(QString::number(vec4MinValue.x));
        lineEditMinY_->setText(QString::number(vec4MinValue.y));
        lineEditMinZ_->setText(QString::number(vec4MinValue.z));
        lineEditMinW_->setText(QString::number(vec4MinValue.w));
        lineEditMaxX_->setText(QString::number(vec4MaxValue.x));
        lineEditMaxY_->setText(QString::number(vec4MaxValue.y));
        lineEditMaxZ_->setText(QString::number(vec4MaxValue.z));
        lineEditMaxW_->setText(QString::number(vec4MaxValue.w));
        lineEditIcrementX_->setText(QString::number(vec4Increment.x));
        lineEditIcrementY_->setText(QString::number(vec4Increment.y));
        lineEditIcrementZ_->setText(QString::number(vec4Increment.z));
        lineEditIcrementW_->setText(QString::number(vec4Increment.w));
        return;
    }

    LogWarn("No settings widget for " + property_->getIdentifier() + " found.")
}

void PropertySettingsWidgetQt::save() {
    if (floatProperty_!=NULL) {
        floatProperty_->setMaxValue(lineEditMaxX_->text().toFloat());
        floatProperty_->setMinValue(lineEditMinX_->text().toFloat());
        floatProperty_->setIncrement(lineEditIcrementX_->text().toFloat());
    }

    if (intProperty_ != NULL) {
        intProperty_->setMaxValue(lineEditMaxX_->text().toInt());
        intProperty_->setMinValue(lineEditMinX_->text().toInt());
        intProperty_->setIncrement(lineEditIcrementX_->text().toInt());
    }

    if (floatVec2Property_!=NULL) {
        vec2 vec2MinValue = floatVec2Property_->getMinValue();
        vec2 vec2MaxValue = floatVec2Property_->getMaxValue();
        vec2 vec2Increment = floatVec2Property_->getIncrement();
        vec2MinValue.x = lineEditMinX_->text().toFloat();
        vec2MinValue.y = lineEditMinY_->text().toFloat();
        vec2MaxValue.x = lineEditMaxX_->text().toFloat();
        vec2MaxValue.y = lineEditMaxY_->text().toFloat();
        vec2Increment.x = lineEditIcrementX_->text().toFloat();
        vec2Increment.y = lineEditIcrementY_->text().toFloat();
        floatVec2Property_->setMinValue(vec2MinValue);
        floatVec2Property_->setMaxValue(vec2MaxValue);
        floatVec2Property_->setIncrement(vec2Increment);
    }

    if (floatVec3Property_!=NULL) {
        vec3 vec3MinValue = floatVec3Property_->getMinValue();
        vec3 vec3MaxValue = floatVec3Property_->getMaxValue();
        vec3 vec3Increment = floatVec3Property_->getIncrement();
        vec3MinValue.x = lineEditMinX_->text().toFloat();
        vec3MinValue.y = lineEditMinY_->text().toFloat();
        vec3MinValue.z = lineEditMinZ_->text().toFloat();
        vec3MaxValue.x = lineEditMaxX_->text().toFloat();
        vec3MaxValue.y = lineEditMaxY_->text().toFloat();
        vec3MaxValue.z = lineEditMaxZ_->text().toFloat();
        vec3Increment.x = lineEditIcrementX_->text().toFloat();
        vec3Increment.y = lineEditIcrementY_->text().toFloat();
        vec3Increment.z = lineEditIcrementZ_->text().toFloat();
        floatVec3Property_->setMinValue(vec3MinValue);
        floatVec3Property_->setMaxValue(vec3MaxValue);
        floatVec3Property_->setIncrement(vec3Increment);
    }

    if (floatVec4Property_!=NULL) {
        vec4 vec4MinValue = floatVec4Property_->getMinValue();
        vec4 vec4MaxValue = floatVec4Property_->getMaxValue();
        vec4 vec4Increment = floatVec4Property_->getIncrement();
        vec4MinValue.x = lineEditMinX_->text().toFloat();
        vec4MinValue.y = lineEditMinY_->text().toFloat();
        vec4MinValue.z = lineEditMinZ_->text().toFloat();
        vec4MinValue.w = lineEditMinW_->text().toFloat();
        vec4MaxValue.x = lineEditMaxX_->text().toFloat();
        vec4MaxValue.y = lineEditMaxY_->text().toFloat();
        vec4MaxValue.z = lineEditMaxZ_->text().toFloat();
        vec4MaxValue.w = lineEditMaxW_->text().toFloat();
        vec4Increment.x = lineEditIcrementX_->text().toFloat();
        vec4Increment.y = lineEditIcrementY_->text().toFloat();
        vec4Increment.z = lineEditIcrementZ_->text().toFloat();
        vec4Increment.w = lineEditIcrementW_->text().toFloat();
        floatVec4Property_->setMinValue(vec4MinValue);
        floatVec4Property_->setMaxValue(vec4MaxValue);
        floatVec4Property_->setIncrement(vec4Increment);
    }

    if (intVec2Property_ != NULL) {
        ivec2 vec2MinValue = intVec2Property_->getMinValue();
        ivec2 vec2MaxValue = intVec2Property_->getMaxValue();
        ivec2 vec2Increment = intVec2Property_->getIncrement();
        vec2MinValue.x = lineEditMinX_->text().toInt();
        vec2MinValue.y = lineEditMinY_->text().toInt();
        vec2MaxValue.x = lineEditMaxX_->text().toInt();
        vec2MaxValue.y = lineEditMaxY_->text().toInt();
        vec2Increment.x = lineEditIcrementX_->text().toInt();
        vec2Increment.y = lineEditIcrementY_->text().toInt();
        intVec2Property_->setMinValue(vec2MinValue);
        intVec2Property_->setMaxValue(vec2MaxValue);
        intVec2Property_->setIncrement(vec2Increment);
    }

    if (intVec3Property_ != NULL) {
        ivec3 vec3MinValue = intVec3Property_->getMinValue();
        ivec3 vec3MaxValue = intVec3Property_->getMaxValue();
        ivec3 vec3Increment = intVec3Property_->getIncrement();
        vec3MinValue.x = lineEditMinX_->text().toInt();
        vec3MinValue.y = lineEditMinY_->text().toInt();
        vec3MinValue.z = lineEditMinZ_->text().toInt();
        vec3MaxValue.x = lineEditMaxX_->text().toInt();
        vec3MaxValue.y = lineEditMaxY_->text().toInt();
        vec3MaxValue.z = lineEditMaxZ_->text().toInt();
        vec3Increment.x = lineEditIcrementX_->text().toInt();
        vec3Increment.y = lineEditIcrementY_->text().toInt();
        vec3Increment.z = lineEditIcrementZ_->text().toInt();
        intVec3Property_->setMinValue(vec3MinValue);
        intVec3Property_->setMaxValue(vec3MaxValue);
        intVec3Property_->setIncrement(vec3Increment);
    }

    if (intVec4Property_ != NULL) {
        ivec4 vec4MinValue = intVec4Property_->getMinValue();
        ivec4 vec4MaxValue = intVec4Property_->getMaxValue();
        ivec4 vec4Increment = intVec4Property_->getIncrement();
        vec4MinValue.x = lineEditMinX_->text().toInt();
        vec4MinValue.y = lineEditMinY_->text().toInt();
        vec4MinValue.z = lineEditMinZ_->text().toInt();
        vec4MinValue.w = lineEditMinW_->text().toInt();
        vec4MaxValue.x = lineEditMaxX_->text().toInt();
        vec4MaxValue.y = lineEditMaxY_->text().toInt();
        vec4MaxValue.z = lineEditMaxZ_->text().toInt();
        vec4MaxValue.w = lineEditMaxW_->text().toInt();
        vec4Increment.x = lineEditIcrementX_->text().toInt();
        vec4Increment.y = lineEditIcrementY_->text().toInt();
        vec4Increment.z = lineEditIcrementZ_->text().toInt();
        vec4Increment.w = lineEditIcrementW_->text().toInt();
        intVec4Property_->setMinValue(vec4MinValue);
        intVec4Property_->setMaxValue(vec4MaxValue);
        intVec4Property_->setIncrement(vec4Increment);
    }

    property_->updateWidgets();
    hide();
}





}//namespace