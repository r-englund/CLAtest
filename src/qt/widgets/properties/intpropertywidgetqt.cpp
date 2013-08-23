#include <inviwo/qt/widgets/properties/intpropertywidgetqt.h>

#include <QHBoxLayout>
#include <QLabel>

namespace inviwo {

IntPropertyWidgetQt::IntPropertyWidgetQt(IntProperty* property) : property_(property) {
    generateWidget();
    updateFromProperty();
}

void IntPropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    label_ = new EditableLabelQt(property_->getDisplayName());
    hLayout->addWidget(label_);
    sliderWidget_ = new IntSliderWidgetQt(property_->getMinValue(), property_->getMaxValue(), property_->getIncrement());
    connect(label_, SIGNAL(textChanged()),this, SLOT(setPropertyDisplayName()));
    connect(sliderWidget_, SIGNAL(valueChanged(int)), this, SLOT(setPropertyValue()));
    hLayout->addWidget(sliderWidget_);
    hLayout->setContentsMargins(0,0,0,0);
    hLayout->setSpacing(0);
    setLayout(hLayout);
    generatesSettingsWidget();
}

void IntPropertyWidgetQt::updateFromProperty() {
    sliderWidget_->setRange(property_->getMinValue(), property_->getMaxValue());
    sliderWidget_->setValue(property_->get());
    sliderWidget_->setIncrement(property_->getIncrement());
}


void IntPropertyWidgetQt::showContextMenu( const QPoint& pos ) {
    
    QPoint globalPos = sliderWidget_->mapToGlobal(pos);

    QAction* selecteditem = settingsMenu_->exec(globalPos);
    if (selecteditem == settingsMenu_->actions().at(0)) {
        settingsWidget_->reload();
        settingsWidget_->show();
    }
    else if (selecteditem == settingsMenu_->actions().at(1)) {
        //Set current value of the slider to min value of the property
        property_->setMinValue(sliderWidget_->getValue());
        updateFromProperty();
    }
    else if (selecteditem == settingsMenu_->actions().at(2)){
        //Set current value of the slider to max value of the property
        property_->setMaxValue(sliderWidget_->getValue());
        updateFromProperty();
    }
}

void IntPropertyWidgetQt::generatesSettingsWidget() {
    settingsWidget_ = new PropertySettingsWidgetQt(property_);
    settingsMenu_ = new QMenu();
    settingsMenu_->addAction("Property settings");
    settingsMenu_->addAction("Set as Min");
    settingsMenu_->addAction("Set as Max");
    sliderWidget_->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(sliderWidget_,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showContextMenu(const QPoint&)));
}

void IntPropertyWidgetQt::setPropertyValue(){
   property_->set(sliderWidget_->getValue());
   emit modified();
}

void IntPropertyWidgetQt::setPropertyDisplayName(){
    property_->setDisplayName(label_->getText());
}

} // namespace
