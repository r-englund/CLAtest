#include <inviwo/qt/widgets/properties/intvec2propertywidgetqt.h>

namespace inviwo {

IntVec2PropertyWidgetQt::IntVec2PropertyWidgetQt(IntVec2Property *property) : property_(property) {
	generateWidget();
    generatesSettingsWidget();
	updateFromProperty();
	}


void IntVec2PropertyWidgetQt::generateWidget() {
	QHBoxLayout* hLayout = new QHBoxLayout();
	QWidget* sliderWidget = new QWidget();
	QVBoxLayout* vLayout = new QVBoxLayout();

    sliderWidget->setLayout(vLayout);
    valueVecMax_ = property_->getMaxValue();
    valueVecMin_ = property_->getMinValue();
    valueIncrement_ = property_->getIncrement();
    sliderX_ = new IntSliderWidgetQt(valueVecMin_.x,valueVecMax_.x,valueIncrement_.x);  
    sliderY_ = new IntSliderWidgetQt(valueVecMin_.y,valueVecMax_.y,valueIncrement_.y);

    vLayout->addWidget(sliderX_);
    vLayout->addWidget(sliderY_);
    
    hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
    hLayout->addWidget(sliderWidget);
    setLayout(hLayout);

    connect(sliderX_, SIGNAL(valueChanged(int)), this, SLOT(setPropertyValue()));
    connect(sliderY_, SIGNAL(valueChanged(int)), this, SLOT(setPropertyValue()));
   
}
void IntVec2PropertyWidgetQt::generatesSettingsWidget(){
    settingsWidget_ = new PropertySettingsWidgetQt(property_);

    settingsMenu_ = new QMenu();
    settingsMenu_->addAction("Property settings");
    settingsMenu_->addAction("Set as Min");
    settingsMenu_->addAction("Set as Max");

    sliderX_->setContextMenuPolicy(Qt::CustomContextMenu);
    sliderY_->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(sliderX_,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showContextMenuX(const QPoint&)));
    connect(sliderY_,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showContextMenuY(const QPoint&)));

}


void IntVec2PropertyWidgetQt::updateFromProperty() {
    valueVec_ = property_->get();
    valueVecMax_ = property_->getMaxValue();
    valueVecMin_ = property_->getMinValue();
    valueIncrement_ = property_->getIncrement();

    sliderX_->setRange(valueVecMin_.x,valueVecMax_.x);
    sliderY_->setRange(valueVecMin_.y,valueVecMax_.y);

    sliderX_->setIncrement(valueIncrement_.x);
    sliderY_->setIncrement(valueIncrement_.y);

    sliderX_->setValue(valueVec_.x);
    sliderY_->setValue(valueVec_.y);
};

void IntVec2PropertyWidgetQt::setPropertyValue(){
    valueVec_ = property_->get();
    valueVec_.x = sliderX_->getValue();
    valueVec_.y = sliderY_->getValue();
    property_->set(valueVec_);
    emit modified();
}

void IntVec2PropertyWidgetQt::showContextMenuX( const QPoint& pos ) {
    QPoint globalPos = sliderX_->mapToGlobal(pos);
    QAction* selecteditem = settingsMenu_->exec(globalPos);
    if (selecteditem == settingsMenu_->actions().at(0)) {
        settingsWidget_->reload();
        settingsWidget_->show();
    }
    else if (selecteditem == settingsMenu_->actions().at(1)) {
        //Set current value of the slider to min value of the property
        valueVecMin_ = property_->getMinValue();
        valueVecMin_.x = sliderX_->getValue();
        std::cout << "Slider value :" << sliderX_->getValue() << std::endl;
        property_->setMinValue(valueVecMin_);
        updateFromProperty();
    }
    else if (selecteditem == settingsMenu_->actions().at(2)){
        //Set current value of the slider to max value of the property
        valueVecMax_ = property_->getMaxValue();
        valueVecMax_.x = sliderX_->getValue();
        property_->setMaxValue(valueVecMax_);
        updateFromProperty();
    }

}

void IntVec2PropertyWidgetQt::showContextMenuY( const QPoint& pos ) {
    QPoint globalPos = sliderY_->mapToGlobal(pos);

    QAction* selecteditem = settingsMenu_->exec(globalPos);
    if (selecteditem == settingsMenu_->actions().at(0)) {
        settingsWidget_->reload();
        settingsWidget_->show();
    }
    else if (selecteditem == settingsMenu_->actions().at(1)) {
        //Set current value of the slider to min value of the property
        valueVecMin_ = property_->getMinValue();
        valueVecMin_.y = sliderY_->getValue();
        property_->setMinValue(valueVecMin_);
        updateFromProperty();
    }
    else if (selecteditem == settingsMenu_->actions().at(2)){
        //Set current value of the slider to max value of the property
        valueVecMax_ = property_->getMaxValue();
        valueVecMax_.y = sliderY_->getValue();
        property_->setMaxValue(valueVecMax_);
        updateFromProperty();
    }

}


} //namespace