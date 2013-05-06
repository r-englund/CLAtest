#include <inviwo/qt/widgets/properties/floatvec3propertywidgetqt.h>

#include <QHBoxLayout>
#include <QLabel>

namespace inviwo {

FloatVec3PropertyWidgetQt::FloatVec3PropertyWidgetQt(FloatVec3Property* property) : property_(property),
valueVec3Min_(property->getMinValue()),
valueVec3Max_(property->getMaxValue()),
valueIncrement_(property->getIncrement()){
    generateWidget();
    updateFromProperty();
    generatesSettingsWidget();
}

void FloatVec3PropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));

    QWidget* sliderWidget = new QWidget();
    QVBoxLayout* vLayout = new QVBoxLayout();
    sliderWidget->setLayout(vLayout);

 /*   sliderX_ = new FloatSliderWidgetQt(valueVec3Min_.x,valueVec3Max_.x,valueIncrement_.x);
    sliderY_ = new FloatSliderWidgetQt(valueVec3Min_.y,valueVec3Max_.y,valueIncrement_.y);
    sliderZ_ = new FloatSliderWidgetQt(valueVec3Min_.z,valueVec3Max_.z,valueIncrement_.z);*/

    sliderX_ = new FloatSliderWidgetQt();
    sliderY_ = new FloatSliderWidgetQt();
    sliderZ_ = new FloatSliderWidgetQt();

    vLayout->addWidget(sliderX_);
    vLayout->addWidget(sliderY_);
    vLayout->addWidget(sliderZ_);
    hLayout->addWidget(sliderWidget);
    setLayout(hLayout);

    
    connect(sliderX_, SIGNAL(valueChanged(float)), this, SLOT(setPropertyValue()));
    connect(sliderY_, SIGNAL(valueChanged(float)), this, SLOT(setPropertyValue()));
    connect(sliderZ_, SIGNAL(valueChanged(float)), this, SLOT(setPropertyValue()));
    
}

void FloatVec3PropertyWidgetQt::updateFromProperty() {
    valueVec3Max_ = property_->getMaxValue();
    valueVec3Min_ = property_->getMinValue();
    valueIncrement_ = property_->getIncrement();
    valueVec3_ = property_->get();

    sliderX_->initValue(valueVec3_.x);
    sliderY_->initValue(valueVec3_.y);
    sliderZ_->initValue(valueVec3_.z);

    sliderX_->setRange(valueVec3Min_.x,valueVec3Max_.x);
    sliderY_->setRange(valueVec3Min_.y,valueVec3Max_.y);
    sliderZ_->setRange(valueVec3Min_.z,valueVec3Max_.z);

    sliderX_->setValue(valueVec3_.x);
    sliderY_->setValue(valueVec3_.y);
    sliderZ_->setValue(valueVec3_.z);

    sliderX_->setIncrement(valueIncrement_.x);
    sliderY_->setIncrement(valueIncrement_.y);
    sliderZ_->setIncrement(valueIncrement_.z);
}


void FloatVec3PropertyWidgetQt::generatesSettingsWidget() {
    settingsWidget_ = new PropertySettingsWidgetQt(property_);

    settingsMenu_ = new QMenu();
    settingsMenu_->addAction("Property settings");
    settingsMenu_->addAction("Set as Min");
    settingsMenu_->addAction("Set as Max");

    sliderX_->setContextMenuPolicy(Qt::CustomContextMenu);
    sliderY_->setContextMenuPolicy(Qt::CustomContextMenu);
    sliderZ_->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(sliderX_,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showContextMenuX(const QPoint&)));
    connect(sliderY_,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showContextMenuY(const QPoint&)));
    connect(sliderZ_,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showContextMenuZ(const QPoint&)));
}

void FloatVec3PropertyWidgetQt::showContextMenuX( const QPoint& pos ) {
    
    QPoint globalPos = sliderX_->mapToGlobal(pos);
    QAction* selecteditem = settingsMenu_->exec(globalPos);
    if (selecteditem == settingsMenu_->actions().at(0)) {
        settingsWidget_->reload();
        settingsWidget_->show();
    }
    else if (selecteditem == settingsMenu_->actions().at(1)) {
        //Set current value of the slider to min value of the property
        valueVec3Min_ = property_->getMinValue();
        valueVec3Min_.x = sliderX_->getValue();
        property_->setMinValue(valueVec3Min_);
        updateFromProperty();
    }
    else if (selecteditem == settingsMenu_->actions().at(2)){
        //Set current value of the slider to max value of the property
        valueVec3Max_ = property_->getMaxValue();
        valueVec3Max_.x = sliderX_->getValue();
        property_->setMaxValue(valueVec3Max_);
        updateFromProperty();
    }
}

void FloatVec3PropertyWidgetQt::showContextMenuY( const QPoint& pos ) {
    QPoint globalPos = sliderY_->mapToGlobal(pos);

    QAction* selecteditem = settingsMenu_->exec(globalPos);
    if (selecteditem == settingsMenu_->actions().at(0)) {
        settingsWidget_->reload();
        settingsWidget_->show();
    }
    else if (selecteditem == settingsMenu_->actions().at(1)) {
        //Set current value of the slider to min value of the property
        valueVec3Min_ = property_->getMinValue();
        valueVec3Min_.y = sliderY_->getValue();
        property_->setMinValue(valueVec3Min_);
        updateFromProperty();
    }
    else if (selecteditem == settingsMenu_->actions().at(2)){
        //Set current value of the slider to max value of the property
        valueVec3Max_ = property_->getMaxValue();
        valueVec3Max_.y = sliderY_->getValue();
        property_->setMaxValue(valueVec3Max_);
        updateFromProperty();
    }
}

void FloatVec3PropertyWidgetQt::showContextMenuZ( const QPoint& pos ) {
    QPoint globalPos = sliderZ_->mapToGlobal(pos);

    QAction* selecteditem = settingsMenu_->exec(globalPos);
    if (selecteditem == settingsMenu_->actions().at(0)) {
        settingsWidget_->reload();
        settingsWidget_->show();
    }
    else if (selecteditem == settingsMenu_->actions().at(1)) {
        //Set current value of the slider to min value of the property
        valueVec3Min_ = property_->getMinValue();
        valueVec3Min_.z = sliderZ_->getValue();
        property_->setMinValue(valueVec3Min_);
        updateFromProperty();
    }
    else if (selecteditem == settingsMenu_->actions().at(2)){
        //Set current value of the slider to max value of the property
        valueVec3Max_ = property_->getMaxValue();
        valueVec3Max_.z = sliderZ_->getValue();
        property_->setMaxValue(valueVec3Max_);
        updateFromProperty();
    }

}

void FloatVec3PropertyWidgetQt::setPropertyValue() {
    valueVec3_ = property_->get();
    valueVec3_.x = sliderX_->getValue();
    valueVec3_.y = sliderY_->getValue();
    valueVec3_.z = sliderZ_->getValue();
    property_->set(valueVec3_);
}

} // namespace