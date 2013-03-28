#include <inviwo/qt/widgets/properties/floatpropertywidgetqt.h>

namespace inviwo {

FloatPropertyWidgetQt::FloatPropertyWidgetQt(FloatProperty* property) : property_(property) {
    generateWidget();
    updateFromProperty();
}

void FloatPropertyWidgetQt::generateWidget() {
    
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
    sliderWidget_ = new FloatSliderWidgetQt(property_->getMinValue(), property_->getMaxValue(), property_->getIncrement());
    sliderWidget_->setValue(property_->get());
    connect(sliderWidget_->getSlider(), SIGNAL(valueChanged(int)), this, SLOT(setPropertyValueFromSlider()));
    connect (sliderWidget_->getSpinBox(),SIGNAL(valueChanged(double)), this, SLOT(setPropertyValueFromSpinBox()));
    hLayout->addWidget(sliderWidget_);
    setLayout(hLayout);
    
    sliderWidget_->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(sliderWidget_,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showContextMenu(const QPoint&)));

    settingsWidget_ = new PropertySettingsWidgetQt(property_);

}

void FloatPropertyWidgetQt::setPropertyValueFromSlider() {
    sliderWidget_->updateValueSpinBox();
    property_->set(sliderWidget_->getValue());
}
void FloatPropertyWidgetQt::setPropertyValueFromSpinBox() {
    sliderWidget_->updateValueSlider();
    property_->set(sliderWidget_->getValue());
}
    

void FloatPropertyWidgetQt::updateFromProperty() {
    sliderWidget_->setRange(property_->getMinValue(), property_->getMaxValue());
    sliderWidget_->setValue(property_->get());
    sliderWidget_->setIncrement(property_->getIncrement());
    sliderWidget_->updateValueSpinBox();
}

void FloatPropertyWidgetQt::showContextMenu(const QPoint& pos) {

    QPoint globalPos = sliderWidget_->mapToGlobal(pos);

    QMenu settingsMenu;

    settingsMenu.addAction("Property settings");
    settingsMenu.addAction("Set as Min");
    settingsMenu.addAction("Set as Max");

    QAction* selecteditem = settingsMenu.exec(globalPos);
    if (selecteditem == settingsMenu.actions().at(0)) {
        settingsWidget_->show();
    }
    else if (selecteditem == settingsMenu.actions().at(1)) {
        //Set current value of the slider to min value of the property
        property_->setMinValue(sliderWidget_->getValue());
        updateFromProperty();
    }
    else if (selecteditem == settingsMenu.actions().at(2)){
    //Set current value of the slider to max value of the property
        property_->setMaxValue(sliderWidget_->getValue());
        updateFromProperty();
    }
}

} // namespace
