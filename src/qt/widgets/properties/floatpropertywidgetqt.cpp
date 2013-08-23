#include <inviwo/qt/widgets/properties/floatpropertywidgetqt.h>

namespace inviwo {

FloatPropertyWidgetQt::FloatPropertyWidgetQt(FloatProperty* property) : property_(property) {
    generateWidget();
    generatesSettingsWidget();
    updateFromProperty();
}

void FloatPropertyWidgetQt::generateWidget() {    
    QHBoxLayout* hLayout = new QHBoxLayout();
    label_ = new EditableLabelQt(property_->getDisplayName());
    hLayout->addWidget(label_);
    sliderWidget_ = new FloatSliderWidgetQt();    
    hLayout->addWidget(sliderWidget_);
    setLayout(hLayout);

    connect(label_, SIGNAL(textChanged()),this, SLOT(setPropertyDisplayName()));
    connect(sliderWidget_, SIGNAL(valueChanged(float)), this, SLOT(setPropertyValue(float)));
}

void FloatPropertyWidgetQt::setPropertyValue(float value) {
    property_->set(value);
    emit modified();
}

void FloatPropertyWidgetQt::updateFromProperty() {
    float value = property_->get();
    sliderWidget_->initValue(value);
    sliderWidget_->setRange(property_->getMinValue(), property_->getMaxValue());
    sliderWidget_->setValue(value);
    sliderWidget_->setIncrement(property_->getIncrement());
}

void FloatPropertyWidgetQt::showContextMenu(const QPoint& pos) {
    QPoint globalPos = sliderWidget_->mapToGlobal(pos);

    QAction* selecteditem = settingsMenu_->exec(globalPos);
    if (selecteditem == settingsMenu_->actions().at(0)) {
        settingsWidget_->reload();
        settingsWidget_->show();
    } else if (selecteditem == settingsMenu_->actions().at(1)) {
        // set current value of the slider to min value of the property
        property_->setMinValue(sliderWidget_->getValue());
        updateFromProperty();
    } else if (selecteditem == settingsMenu_->actions().at(2)){
        // set current value of the slider to max value of the property
        property_->setMaxValue(sliderWidget_->getValue());
        updateFromProperty();
    }
}

void FloatPropertyWidgetQt::generatesSettingsWidget() {
    settingsWidget_ = new PropertySettingsWidgetQt(property_);  
    settingsMenu_ = new QMenu();
    settingsMenu_->addAction("Property settings");
    settingsMenu_->addAction("Set as Min");
    settingsMenu_->addAction("Set as Max");
    sliderWidget_->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(sliderWidget_, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(showContextMenu(const QPoint&)));
}

void FloatPropertyWidgetQt::setPropertyDisplayName(){
    property_->setDisplayName(label_->getText());
}

} // namespace
