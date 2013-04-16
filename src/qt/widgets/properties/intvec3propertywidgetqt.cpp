#include <inviwo/qt/widgets/properties/intvec3propertywidgetqt.h>
#include <QHBoxLayout>
#include <QLabel>

namespace inviwo {

IntVec3PropertyWidgetQt::IntVec3PropertyWidgetQt(IntVec3Property *property) : property_(property) {
	generateWidget();
    generatesSettingsWidget();
	updateFromProperty();
	}


void IntVec3PropertyWidgetQt::generateWidget() {
	QHBoxLayout* hLayout = new QHBoxLayout();
	hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));

	QWidget* sliderWidget = new QWidget();
	QVBoxLayout* vLayout = new QVBoxLayout();
    sliderWidget->setLayout(vLayout);
    valueVecMax_ = property_->getMaxValue();
    valueVecMin_ = property_->getMinValue();
    valueIncrement_ = property_->getIncrement();
    sliderX_ = new IntSliderWidgetQt(valueVecMin_.x,valueVecMax_.x,valueIncrement_.x);  
    sliderY_ = new IntSliderWidgetQt(valueVecMin_.y,valueVecMax_.y,valueIncrement_.y);
    sliderZ_ = new IntSliderWidgetQt(valueVecMin_.z,valueVecMax_.z,valueIncrement_.z);

    vLayout->addWidget(sliderX_);
    vLayout->addWidget(sliderY_);
    vLayout->addWidget(sliderZ_);
    hLayout->addWidget(sliderWidget);
    setLayout(hLayout);

    connect(sliderX_->getSlider(), SIGNAL(valueChanged(int)), this, SLOT(setPropertyValueFromSlider()));
    connect(sliderY_->getSlider(), SIGNAL(valueChanged(int)), this, SLOT(setPropertyValueFromSlider()));
    connect(sliderZ_->getSlider(), SIGNAL(valueChanged(int)), this, SLOT(setPropertyValueFromSlider()));
    connect (sliderX_->getSpinBox(),SIGNAL(valueChanged(int)), this, SLOT(setPropertyValueFromSpinBox()));
    connect (sliderY_->getSpinBox(),SIGNAL(valueChanged(int)), this, SLOT(setPropertyValueFromSpinBox()));
    connect (sliderZ_->getSpinBox(),SIGNAL(valueChanged(int)), this, SLOT(setPropertyValueFromSpinBox()));
}

void IntVec3PropertyWidgetQt::generatesSettingsWidget(){

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

void IntVec3PropertyWidgetQt::updateFromProperty() {
    valueVec_ = property_->get();
    valueVecMax_ = property_->getMaxValue();
    valueVecMin_ = property_->getMinValue();
    valueIncrement_ = property_->getIncrement();

    sliderX_->setRange(valueVecMin_.x,valueVecMax_.x);
    sliderY_->setRange(valueVecMin_.y,valueVecMax_.y);
    sliderZ_->setRange(valueVecMin_.z,valueVecMax_.z);

    sliderX_->setIncrement(valueIncrement_.x);
    sliderY_->setIncrement(valueIncrement_.y);
    sliderZ_->setIncrement(valueIncrement_.z);

    sliderX_->setValue(valueVec_.x);
    sliderY_->setValue(valueVec_.y);
    sliderZ_->setValue(valueVec_.z);;

    sliderX_->updateValueSpinBox();
    sliderY_->updateValueSpinBox();
    sliderZ_->updateValueSpinBox();
};


void IntVec3PropertyWidgetQt::setPropertyValueFromSlider() {
    sliderX_->updateValueSpinBox();
    sliderY_->updateValueSpinBox();
    sliderZ_->updateValueSpinBox();


    valueVec_ = property_->get();
    valueVec_.x = sliderX_->getValue();
    valueVec_.y = sliderY_->getValue();
    valueVec_.z = sliderZ_->getValue();;
    property_->set(valueVec_);

}

void IntVec3PropertyWidgetQt::setPropertyValueFromSpinBox() {
    sliderX_->updateValueSlider();
    sliderY_->updateValueSlider();
    sliderZ_->updateValueSlider();

    valueVec_ = property_->get();
    valueVec_.x = sliderX_->getValue();
    valueVec_.y = sliderY_->getValue();
    valueVec_.z = sliderZ_->getValue();
    property_->set(valueVec_);

}

void IntVec3PropertyWidgetQt::showContextMenuX( const QPoint& pos ) {
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

void IntVec3PropertyWidgetQt::showContextMenuY( const QPoint& pos ) {
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

void IntVec3PropertyWidgetQt::showContextMenuZ( const QPoint& pos ) {
    QPoint globalPos = sliderZ_->mapToGlobal(pos);

    QAction* selecteditem = settingsMenu_->exec(globalPos);
    if (selecteditem == settingsMenu_->actions().at(0)) {
        settingsWidget_->reload();
        settingsWidget_->show();
    }
    else if (selecteditem == settingsMenu_->actions().at(1)) {
        //Set current value of the slider to min value of the property
        valueVecMin_ = property_->getMinValue();
        valueVecMin_.z = sliderZ_->getValue();
        property_->setMinValue(valueVecMin_);
        updateFromProperty();
    }
    else if (selecteditem == settingsMenu_->actions().at(2)){
        //Set current value of the slider to max value of the property
        valueVecMax_ = property_->getMaxValue();
        valueVecMax_.z = sliderZ_->getValue();
        property_->setMaxValue(valueVecMax_);
        updateFromProperty();
    }
}


} //namespace