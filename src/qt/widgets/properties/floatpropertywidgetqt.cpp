#include <inviwo/qt/widgets/properties/floatpropertywidgetqt.h>

namespace inviwo {

FloatPropertyWidgetQt::FloatPropertyWidgetQt(FloatProperty* property) : property_(property) {
    PropertyWidgetQt::setProperty(property_);
    PropertyWidgetQt::generateContextMenu();
    generateWidget();
    updateFromProperty();
    generatesSettingsWidget();
}

void FloatPropertyWidgetQt::generateWidget() {    
    QHBoxLayout* hLayout = new QHBoxLayout();
    if (property_->getReadOnly()) {
        hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
        readOnlyLabel_ = new QLabel();
        hLayout->addWidget(readOnlyLabel_);
        setLayout(hLayout);
    }
    else {
        label_ = new EditableLabelQt(property_->getDisplayName(),PropertyWidgetQt::generatePropertyWidgetMenu());
        hLayout->addWidget(label_);

        sliderWidget_ = new FloatSliderWidgetQt();    
        hLayout->addWidget(sliderWidget_);
        setLayout(hLayout);

        QSizePolicy labelPol = label_->sizePolicy();
        labelPol.setHorizontalStretch(1);
        label_->setSizePolicy(labelPol);

        QSizePolicy sliderPol = sliderWidget_->sizePolicy();
        sliderPol.setHorizontalStretch(3);
        sliderWidget_->setSizePolicy(sliderPol);
 

        connect(label_, SIGNAL(textChanged()),this, SLOT(setPropertyDisplayName()));
        connect(sliderWidget_, SIGNAL(valueChanged(float)), this, SLOT(setPropertyValue(float)));
    }
}

void FloatPropertyWidgetQt::setPropertyValue(float value) {
    property_->set(value);
    emit modified();
}

void FloatPropertyWidgetQt::updateFromProperty() {
    float value = property_->get();
    if (property_->getReadOnly()) {
        readOnlyLabel_->setText(QString::number(property_->get()));
        readOnlyLabel_->setToolTip("Min: " +QString::number(property_->getMinValue())+
                                 "  Max: " +QString::number(property_->getMaxValue()));
    }
    else{
        sliderWidget_->initValue(value);
        sliderWidget_->setRange(property_->getMinValue(), property_->getMaxValue());    
        sliderWidget_->setIncrement(property_->getIncrement());
        sliderWidget_->setValue(value);
    }
}

void FloatPropertyWidgetQt::showContextMenuSlider(const QPoint& pos) {
    InviwoApplication* inviwoApp = InviwoApplication::getPtr();
    PropertyVisibility::VisibilityMode appVisibilityMode  = static_cast<PropertyVisibility::VisibilityMode>(static_cast<OptionPropertyInt*>(inviwoApp->getSettings()->getPropertyByIdentifier("viewMode"))->get());
    if (appVisibilityMode == PropertyVisibility::DEVELOPMENT) {

        QPoint globalPos = sliderWidget_->mapToGlobal(pos);

        QAction* selecteditem = settingsMenu_->exec(globalPos);
        if (selecteditem == settingsMenu_->actions().at(1)) {
            settingsWidget_->reload();
            settingsWidget_->show();
        } else if (selecteditem == settingsMenu_->actions().at(2)) {
            // set current value of the slider to min value of the property
            property_->setMinValue(sliderWidget_->getValue());
            updateFromProperty();
        } else if (selecteditem == settingsMenu_->actions().at(3)){
            // set current value of the slider to max value of the property
            property_->setMaxValue(sliderWidget_->getValue());
            updateFromProperty();
        }
    }
}

void FloatPropertyWidgetQt::generatesSettingsWidget() {
    settingsWidget_ = new PropertySettingsWidgetQt(property_);  

    settingsMenu_ = PropertyWidgetQt::generatePropertyWidgetMenu();
    settingsMenu_->addAction("Property settings");
    settingsMenu_->addAction("Set as Min");
    settingsMenu_->addAction("Set as Max");

    sliderWidget_->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(sliderWidget_, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(showContextMenuSlider(const QPoint&)));
}

void FloatPropertyWidgetQt::setPropertyDisplayName(){
    property_->setDisplayName(label_->getText());
}

} // namespace
