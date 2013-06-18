#include <inviwo/qt/widgets/properties/floatminmaxpropertywidgetqt.h>
#include <QHBoxLayout>
#include <QLabel>

namespace inviwo {

FloatMinMaxPropertyWidgetQt::FloatMinMaxPropertyWidgetQt(FloatMinMaxProperty *property) : property_(property),
valueMinMaxMin_(property->getMinValue()),
valueMinMaxMax_(property->getMaxValue()),
valueIncrement_(property->getIncrement()) {
	generateWidget();
    generatesSettingsWidget();
	updateFromProperty();
	}


void FloatMinMaxPropertyWidgetQt::generateWidget() {
	QHBoxLayout* hLayout = new QHBoxLayout();
	hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));

	QWidget* sliderWidget = new QWidget();
	QVBoxLayout* vLayout = new QVBoxLayout();
	sliderWidget->setLayout(vLayout);

    slider_ = new RangeFloatSliderQt(this);

	vLayout->addWidget(slider_);
	hLayout->addWidget(sliderWidget);
	setLayout(hLayout);

    connect(slider_, SIGNAL(valueChanged(float)), this, SLOT(setPropertyValue()));
}

void FloatMinMaxPropertyWidgetQt::updateFromProperty() {
    valueMinMaxMax_ = property_->getMaxValue();
    valueMinMaxMin_ = property_->getMinValue();
    valueIncrement_ = property_->getIncrement();
    valueMinMax_ = property_->get();
    /*sliderX_->initValue(valueMinMax_.x);
    sliderY_->initValue(valueMinMax_.y);

    sliderX_->setRange(valueMinMaxMin_.x,valueMinMaxMax_.x);
    sliderY_->setRange(valueMinMaxMin_.y,valueMinMaxMax_.y);
    
    sliderX_->setValue(valueMinMax_.x);
    sliderY_->setValue(valueMinMax_.y);

    sliderX_->setIncrement(valueIncrement_.x);
    sliderY_->setIncrement(valueIncrement_.y);*/
}


void FloatMinMaxPropertyWidgetQt::generatesSettingsWidget() {
    settingsWidget_ = new PropertySettingsWidgetQt(property_);

    settingsMenu_ = new QMenu();
    settingsMenu_->addAction("Property settings");
    settingsMenu_->addAction("Set as Min");
    settingsMenu_->addAction("Set as Max");

    /*sliderX_->setContextMenuPolicy(Qt::CustomContextMenu);
    sliderY_->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(sliderX_,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showContextMenuX(const QPoint&)));
    connect(sliderY_,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showContextMenuY(const QPoint&)));*/
}

void FloatMinMaxPropertyWidgetQt::showContextMenuX( const QPoint& pos ) {
    /*QPoint globalPos = sliderX_->mapToGlobal(pos);
    QAction* selecteditem = settingsMenu_->exec(globalPos);
    if (selecteditem == settingsMenu_->actions().at(0)) {
        settingsWidget_->reload();
        settingsWidget_->show();
    }
    else if (selecteditem == settingsMenu_->actions().at(1)) {
        //Set current value of the slider to min value of the property
        valueMinMaxMin_ = property_->getMinValue();
        valueMinMaxMin_.x = sliderX_->getValue();
        property_->setMinValue(valueMinMaxMin_);
        updateFromProperty();
    }
    else if (selecteditem == settingsMenu_->actions().at(2)){
        //Set current value of the slider to max value of the property
        valueMinMaxMax_ = property_->getMaxValue();
        valueMinMaxMax_.x = sliderX_->getValue();
        property_->setMaxValue(valueMinMaxMax_);
        updateFromProperty();
    }*/
}

void FloatMinMaxPropertyWidgetQt::showContextMenuY( const QPoint& pos ) {
    /*QPoint globalPos = sliderY_->mapToGlobal(pos);

    QAction* selecteditem = settingsMenu_->exec(globalPos);
    if (selecteditem == settingsMenu_->actions().at(0)) {
        settingsWidget_->reload();
        settingsWidget_->show();
    }
    else if (selecteditem == settingsMenu_->actions().at(1)) {
        //Set current value of the slider to min value of the property
        valueMinMaxMin_ = property_->getMinValue();
        valueMinMaxMin_.y = sliderY_->getValue();
        property_->setMinValue(valueMinMaxMin_);
        updateFromProperty();
    }
    else if (selecteditem == settingsMenu_->actions().at(2)){
        //Set current value of the slider to max value of the property
        valueMinMaxMax_ = property_->getMaxValue();
        valueMinMaxMax_.y = sliderY_->getValue();
        property_->setMaxValue(valueMinMaxMax_);
        updateFromProperty();
    }*/
}

void FloatMinMaxPropertyWidgetQt::setPropertyValue() {
    valueMinMax_ = property_->get();
    //valueMinMax_.x = sliderX_->getValue();
    //valueMinMax_.y = sliderY_->getValue();
    property_->set(valueMinMax_);
    emit modified();
}

} //namespace