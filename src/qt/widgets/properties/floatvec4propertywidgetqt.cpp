#include <inviwo/qt/widgets/properties/floatvec4propertywidgetqt.h>

#include <QHBoxLayout>
#include <QLabel>

namespace inviwo {

FloatVec4PropertyWidgetQt::FloatVec4PropertyWidgetQt(FloatVec4Property* property) : property_(property) {
    generateWidget();
    generatesSettingsWidget();
    updateFromProperty();
}

void FloatVec4PropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));

    QWidget* sliderWidget = new QWidget();
    QVBoxLayout* vLayout = new QVBoxLayout();
    sliderWidget->setLayout(vLayout);
    vec4 valueVec4Max = property_->getMaxValue();
    vec4 valueVec4Min = property_->getMinValue();
    vec4 valueIncrement = property_->getIncrement();
    sliderX_ = new FloatSliderWidgetQt(valueVec4Min.x,valueVec4Max.x,valueIncrement.x);  
    sliderY_ = new FloatSliderWidgetQt(valueVec4Min.y,valueVec4Max.y,valueIncrement.y);
    sliderZ_ = new FloatSliderWidgetQt(valueVec4Min.z,valueVec4Max.z,valueIncrement.z);
	sliderW_ = new FloatSliderWidgetQt(valueVec4Min.z,valueVec4Max.z,valueIncrement.z);
   
    vLayout->addWidget(sliderX_);
    vLayout->addWidget(sliderY_);
    vLayout->addWidget(sliderZ_);
    vLayout->addWidget(sliderW_);
    hLayout->addWidget(sliderWidget);
    setLayout(hLayout);

    /*
    connect(sliderX_, SIGNAL(valueChanged(float)), this, SLOT(setPropertyValue()));
    connect(sliderY_, SIGNAL(valueChanged(float)), this, SLOT(setPropertyValue()));
    connect(sliderZ_, SIGNAL(valueChanged(float)), this, SLOT(setPropertyValue()));
    connect(sliderW_, SIGNAL(valueChanged(float)), this, SLOT(setPropertyValue()));
    */
}

void FloatVec4PropertyWidgetQt::setPropertyValue() {
    vec4 valueVec4(sliderX_->getValue(),sliderY_->getValue(),sliderZ_->getValue(),sliderW_->getValue());
    property_->set(valueVec4);
}


void FloatVec4PropertyWidgetQt::updateFromProperty() {

    vec4 valueVec4Max = property_->getMaxValue();
    vec4 valueVec4Min = property_->getMinValue();


    sliderX_->setRange(valueVec4Min.x,valueVec4Max.x);
    sliderY_->setRange(valueVec4Min.y,valueVec4Max.y);
    sliderZ_->setRange(valueVec4Min.z,valueVec4Max.z);
    sliderW_->setRange(valueVec4Min.w,valueVec4Max.w);

    vec4 valueVec4 = property_->get();
    sliderX_->setValue(valueVec4.x);
    sliderY_->setValue(valueVec4.y);
    sliderZ_->setValue(valueVec4.z);
    sliderW_->setValue(valueVec4.w);

    vec4 valueIncrement = property_->getIncrement();
    sliderX_->setIncrement(valueIncrement.x);
    sliderY_->setIncrement(valueIncrement.y);
    sliderZ_->setIncrement(valueIncrement.z);
    sliderW_->setIncrement(valueIncrement.w);
}

void FloatVec4PropertyWidgetQt::generatesSettingsWidget() {
    settingsWidget_ = new PropertySettingsWidgetQt(property_);

    settingsMenu_ = new QMenu();
    settingsMenu_->addAction("Property settings");
    settingsMenu_->addAction("Set as Min");
    settingsMenu_->addAction("Set as Max");

    sliderX_->setContextMenuPolicy(Qt::CustomContextMenu);
    sliderY_->setContextMenuPolicy(Qt::CustomContextMenu);
    sliderZ_->setContextMenuPolicy(Qt::CustomContextMenu);
    sliderW_->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(sliderX_,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showContextMenuX(const QPoint&)));
    connect(sliderY_,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showContextMenuY(const QPoint&)));
    connect(sliderZ_,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showContextMenuZ(const QPoint&)));
    connect(sliderW_,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showContextMenuW(const QPoint&)));

}

void FloatVec4PropertyWidgetQt::showContextMenuX( const QPoint& pos) {

       QPoint globalPos = sliderX_->mapToGlobal(pos);
        QAction* selecteditem = settingsMenu_->exec(globalPos);
        if (selecteditem == settingsMenu_->actions().at(0)) {
            settingsWidget_->reload();
            settingsWidget_->show();
        }
        else if (selecteditem == settingsMenu_->actions().at(1)) {
            //Set current value of the slider to min value of the property
            vec4 vec4MinValue = property_->getMinValue();
            vec4MinValue.x = sliderX_->getValue();
            property_->setMinValue(vec4MinValue);
            updateFromProperty();
        }
        else if (selecteditem == settingsMenu_->actions().at(2)){
            //Set current value of the slider to max value of the property
            vec4 vec4MaxValue = property_->getMaxValue();
            vec4MaxValue.x = sliderX_->getValue();
            property_->setMaxValue(vec4MaxValue);
            updateFromProperty();
        }
  
}

void FloatVec4PropertyWidgetQt::showContextMenuY( const QPoint& pos ) {
    QPoint globalPos = sliderY_->mapToGlobal(pos);

    QAction* selecteditem = settingsMenu_->exec(globalPos);
    if (selecteditem == settingsMenu_->actions().at(0)) {
        settingsWidget_->reload();
        settingsWidget_->show();
    }
    else if (selecteditem == settingsMenu_->actions().at(1)) {
        //Set current value of the slider to min value of the property
        vec4 vec4MinValue = property_->getMinValue();
        vec4MinValue.y = sliderY_->getValue();
        property_->setMinValue(vec4MinValue);
        updateFromProperty();
    }
    else if (selecteditem == settingsMenu_->actions().at(2)){
        //Set current value of the slider to max value of the property
        vec4 vec4MaxValue = property_->getMaxValue();
        vec4MaxValue.y = sliderY_->getValue();
        property_->setMaxValue(vec4MaxValue);
        updateFromProperty();
    }

}

void FloatVec4PropertyWidgetQt::showContextMenuZ( const QPoint& pos ) {
    QPoint globalPos = sliderZ_->mapToGlobal(pos);

    QAction* selecteditem = settingsMenu_->exec(globalPos);
    if (selecteditem == settingsMenu_->actions().at(0)) {
        settingsWidget_->reload();
        settingsWidget_->show();
    }
    else if (selecteditem == settingsMenu_->actions().at(1)) {
        //Set current value of the slider to min value of the property
        vec4 vec4MinValue = property_->getMinValue();
        vec4MinValue.z = sliderZ_->getValue();
        property_->setMinValue(vec4MinValue);
        updateFromProperty();
    }
    else if (selecteditem == settingsMenu_->actions().at(2)){
        //Set current value of the slider to max value of the property
        vec4 vec4MaxValue = property_->getMaxValue();
        vec4MaxValue.z = sliderZ_->getValue();
        property_->setMaxValue(vec4MaxValue);
        updateFromProperty();
    }

}

void FloatVec4PropertyWidgetQt::showContextMenuW( const QPoint& pos ) {
    QPoint globalPos = sliderW_->mapToGlobal(pos);

    QAction* selecteditem = settingsMenu_->exec(globalPos);
    if (selecteditem == settingsMenu_->actions().at(0)) {
        settingsWidget_->reload();
        settingsWidget_->show();
    }
    else if (selecteditem == settingsMenu_->actions().at(1)) {
        //Set current value of the slider to min value of the property
        vec4 vec4MinValue = property_->getMinValue();
        vec4MinValue.w = sliderW_->getValue();
        property_->setMinValue(vec4MinValue);
        updateFromProperty();
    }
    else if (selecteditem == settingsMenu_->actions().at(2)){
        //Set current value of the slider to max value of the property
        vec4 vec4MaxValue = property_->getMaxValue();
        vec4MaxValue.w = sliderW_->getValue();
        property_->setMaxValue(vec4MaxValue);
        updateFromProperty();
    }
}

} // namespace