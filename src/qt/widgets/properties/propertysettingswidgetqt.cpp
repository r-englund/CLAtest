#include <inviwo/qt/widgets/properties/propertysettingswidgetqt.h>

namespace inviwo {

PropertySettingsWidgetQt::PropertySettingsWidgetQt(Property *property) : property_(property),
floatProperty_(NULL),
floatVec2Property_(NULL),
floatVec3Property_(NULL),
floatVec4Property_(NULL),
intProperty_(NULL),
intVec2Property_(NULL),
intVec3Property_(NULL),
intVec4Property_(NULL){

    if (dynamic_cast<FloatProperty*>(property_)) {
        floatProperty_ = static_cast<FloatProperty*>(property_);
    }
    if (dynamic_cast<FloatVec2Property*>(property_)) {
        floatVec2Property_ = static_cast<FloatVec2Property*>(property_);
    }
    if (dynamic_cast<FloatVec3Property*>(property_)) {
        floatVec3Property_ = static_cast<FloatVec3Property*>(property_);
    }
    if (dynamic_cast<FloatVec4Property*>(property_)) {
        floatVec4Property_ = static_cast<FloatVec4Property*>(property_);
    }
    if (dynamic_cast<IntProperty*>(property_)) {
        intProperty_ = static_cast<IntProperty*>(property_);
    }
    if (dynamic_cast<IntVec2Property*>(property_)) {
        intVec2Property_ = static_cast<IntVec2Property*>(property_);
    }
    if (dynamic_cast<IntVec3Property*>(property_)) {
        intVec3Property_ = static_cast<IntVec3Property*>(property_);
    }
    if (dynamic_cast<IntVec4Property*>(property_)) {
        intVec4Property_ = static_cast<IntVec4Property*>(property_);
    }

    generateWidget();
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->setWindowModality(Qt::ApplicationModal);
    this->setFixedSize(300,125);
}


void PropertySettingsWidgetQt::generateWidget() {

    btnApply_ = new QPushButton("Apply");
    btnCancel_ = new QPushButton("Cancel");

    connect(btnApply_,SIGNAL(clicked()),this,SLOT(save()));
    connect(btnCancel_,SIGNAL(clicked()),this,SLOT(cancel()));
    
    gridLayout_ = new QGridLayout();
    lineEditMaxX_ = new QLineEdit();
    lineEditMinX_ = new QLineEdit();
    lineEditIcrementX_ = new QLineEdit();
    if (floatProperty_!= NULL || intProperty_!= NULL ) {
        gridLayout_->setColumnStretch(1,2);
        gridLayout_->setColumnStretch(2,2);
        gridLayout_->addWidget(new QLabel("Min value"),1,1);
        gridLayout_->addWidget(lineEditMinX_,1,2);
        gridLayout_->addWidget(new QLabel("Max value"),2,1);
        gridLayout_->addWidget(lineEditMaxX_,2,2);
        gridLayout_->addWidget(new QLabel("Icrement"),3,1);
        gridLayout_->addWidget(lineEditIcrementX_,3,2);
        gridLayout_->addWidget(btnApply_,4,1);
        gridLayout_->addWidget(btnCancel_,4,2);
    }
    if (floatVec2Property_ != NULL || intVec2Property_ != NULL) {
        generateVec2Widget();
    }
    if (floatVec3Property_ != NULL || intVec3Property_ != NULL) {
        generateVec3Widget();
    }
    if (floatVec4Property_!=NULL || intVec4Property_!=NULL) {
        generateVec4Widget();
    }

    setLayout(gridLayout_);
    reload();

    setWindowTitle(QString::fromStdString(property_->getDisplayName()));
}

void PropertySettingsWidgetQt::generateVec2Widget() {
    lineEditMaxY_ = new QLineEdit();
    lineEditMinY_ = new QLineEdit();
    lineEditIcrementY_ = new QLineEdit();

    gridLayout_->addWidget(lineEditMinX_,2,2,1,1);
    gridLayout_->addWidget(lineEditMinY_,2,3,1,1);

    gridLayout_->addWidget(lineEditMaxX_,3,2,1,1);
    gridLayout_->addWidget(lineEditMaxY_,3,3,1,1);

    gridLayout_->addWidget(lineEditIcrementX_,4,2,1,1);
    gridLayout_->addWidget(lineEditIcrementY_,4,3,1,1);

    gridLayout_->addWidget(new QLabel("X"),1,2,1,1,Qt::AlignHCenter);
    gridLayout_->addWidget(new QLabel("Y"),1,3,1,1,Qt::AlignHCenter);

    gridLayout_->addWidget(new QLabel("Min"),2,1);
    gridLayout_->addWidget(new QLabel("Max"),3,1);
    gridLayout_->addWidget(new QLabel("Increment"),4,1);

    gridLayout_->addWidget(btnApply_,5,1,1,2);
    gridLayout_->addWidget(btnCancel_,5,3,1,2);
}

void PropertySettingsWidgetQt::generateVec3Widget() {
    lineEditMaxY_ = new QLineEdit();
    lineEditMaxZ_ = new QLineEdit();

    lineEditMinY_ = new QLineEdit();
    lineEditMinZ_ = new QLineEdit();

    lineEditIcrementY_ = new QLineEdit();
    lineEditIcrementZ_ = new QLineEdit();

    gridLayout_->addWidget(lineEditMinX_,2,2,1,1);
    gridLayout_->addWidget(lineEditMinY_,2,3,1,1);
    gridLayout_->addWidget(lineEditMinZ_,2,4,1,1);

    gridLayout_->addWidget(lineEditMaxX_,3,2,1,1);
    gridLayout_->addWidget(lineEditMaxY_,3,3,1,1);
    gridLayout_->addWidget(lineEditMaxZ_,3,4,1,1);

    gridLayout_->addWidget(lineEditIcrementX_,4,2,1,1);
    gridLayout_->addWidget(lineEditIcrementY_,4,3,1,1);
    gridLayout_->addWidget(lineEditIcrementZ_,4,4,1,1);

    gridLayout_->addWidget(new QLabel("X"),1,2,1,1,Qt::AlignHCenter);
    gridLayout_->addWidget(new QLabel("Y"),1,3,1,1,Qt::AlignHCenter);
    gridLayout_->addWidget(new QLabel("Z"),1,4,1,1,Qt::AlignHCenter);

    gridLayout_->addWidget(new QLabel("Min"),2,1);
    gridLayout_->addWidget(new QLabel("Max"),3,1);
    gridLayout_->addWidget(new QLabel("Increment"),4,1);

    gridLayout_->addWidget(btnApply_,5,2,1,2);
    gridLayout_->addWidget(btnCancel_,5,4,1,2);
}

void PropertySettingsWidgetQt::generateVec4Widget() {
    lineEditMaxY_ = new QLineEdit();
    lineEditMaxZ_ = new QLineEdit();
    lineEditMaxW_ = new QLineEdit();

    lineEditMinY_ = new QLineEdit();
    lineEditMinZ_ = new QLineEdit();
    lineEditMinW_ = new QLineEdit();

    lineEditIcrementY_ = new QLineEdit();
    lineEditIcrementZ_ = new QLineEdit();
    lineEditIcrementW_ = new QLineEdit();

    gridLayout_->addWidget(lineEditMinX_,2,2,1,1);
    gridLayout_->addWidget(lineEditMinY_,2,3,1,1);
    gridLayout_->addWidget(lineEditMinZ_,2,4,1,1);
    gridLayout_->addWidget(lineEditMinW_,2,5,1,1);

    gridLayout_->addWidget(lineEditMaxX_,3,2,1,1);
    gridLayout_->addWidget(lineEditMaxY_,3,3,1,1);
    gridLayout_->addWidget(lineEditMaxZ_,3,4,1,1);
    gridLayout_->addWidget(lineEditMaxW_,3,5,1,1);

    gridLayout_->addWidget(lineEditIcrementX_,4,2,1,1);
    gridLayout_->addWidget(lineEditIcrementY_,4,3,1,1);
    gridLayout_->addWidget(lineEditIcrementZ_,4,4,1,1);
    gridLayout_->addWidget(lineEditIcrementW_,4,5,1,1);

    gridLayout_->addWidget(new QLabel("X"),1,2,1,1,Qt::AlignHCenter);
    gridLayout_->addWidget(new QLabel("Y"),1,3,1,1,Qt::AlignHCenter);
    gridLayout_->addWidget(new QLabel("Z"),1,4,1,1,Qt::AlignHCenter);
    gridLayout_->addWidget(new QLabel("W"),1,5,1,1,Qt::AlignHCenter);

    gridLayout_->addWidget(new QLabel("Min"),2,1);
    gridLayout_->addWidget(new QLabel("Max"),3,1);
    gridLayout_->addWidget(new QLabel("Increment"),4,1);

    gridLayout_->addWidget(btnApply_,5,2,1,2);
    gridLayout_->addWidget(btnCancel_,5,4,1,2);
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
    property_->updatePropertyWidgets();
    hide();

}





}//namespace