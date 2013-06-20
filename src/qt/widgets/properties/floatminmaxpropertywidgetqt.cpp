#include <inviwo/qt/widgets/properties/floatminmaxpropertywidgetqt.h>
#include <QHBoxLayout>
#include <QLabel>

namespace inviwo {

FloatMinMaxPropertyWidgetQt::FloatMinMaxPropertyWidgetQt(FloatMinMaxProperty *property) : property_(property) {
	generateWidget();
	updateFromProperty();
}

void FloatMinMaxPropertyWidgetQt::generateWidget() {
	QHBoxLayout* hLayout = new QHBoxLayout();
	hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));

    spinBoxMin_ = new QDoubleSpinBox(this);
    spinBoxMin_->setFixedWidth(50);
    hLayout->addWidget(spinBoxMin_);

    slider_ = new RangeSliderQt(Qt::Horizontal, this);
	hLayout->addWidget(slider_);

    spinBoxMax_ = new QDoubleSpinBox(this);
    spinBoxMax_->setFixedWidth(50);
    hLayout->addWidget(spinBoxMax_);
	setLayout(hLayout);

    connect(slider_, SIGNAL(valuesChanged(int,int)), this, SLOT(updateFromSlider(int,int)));
    connect(spinBoxMin_, SIGNAL(valueChanged(double)), this, SLOT(updateFromSpinBoxMin(double)));
    connect(spinBoxMax_, SIGNAL(valueChanged(double)), this, SLOT(updateFromSpinBoxMax(double)));
}

void FloatMinMaxPropertyWidgetQt::updateFromProperty() {
    maxNumberOfValues_ = (property_->getRangeMax()-property_->getRangeMin())/property_->getIncrement();
    slider_->setRange(0, static_cast<int>(maxNumberOfValues_));

    spinBoxMin_->setRange(property_->getRangeMin(), property_->getRangeMax());
    spinBoxMax_->setRange(property_->getRangeMin(), property_->getRangeMax());

    spinBoxMin_->setSingleStep(property_->getIncrement());
    spinBoxMax_->setSingleStep(property_->getIncrement());
    setSpinBoxDecimals(property_->getIncrement());

    sliderMin_ = static_cast<int>((property_->getValueMin()-property_->getRangeMin())*maxNumberOfValues_);
    sliderMax_ = static_cast<int>((property_->getValueMax()-property_->getRangeMin())*maxNumberOfValues_);

    slider_->setValue(sliderMin_, sliderMax_);
    blockSignals(true);
    spinBoxMin_->setValue(property_->getValueMin());
    spinBoxMax_->setValue(property_->getValueMax());
    blockSignals(false);
}

void FloatMinMaxPropertyWidgetQt::updateFromSlider(int valMin, int valMax){
    bool changed = false;
    blockSignals(true);
    double valMinDouble = static_cast<double>(valMin)/static_cast<double>(maxNumberOfValues_);
    double valMaxDouble = static_cast<double>(valMax)/static_cast<double>(maxNumberOfValues_);
    if(sliderMin_ != valMin){
        sliderMin_ = valMin;
        spinBoxMin_->setValue(valMinDouble);
        changed = true;
    }
    if(sliderMax_ != valMax){
        sliderMax_ = valMax;
        spinBoxMax_->setValue(valMaxDouble);
        changed = true;
    }
    blockSignals(false);
    if(changed)
        setPropertyValue(valMinDouble, valMaxDouble);
}

void FloatMinMaxPropertyWidgetQt::updateFromSpinBoxMin(double val){
    slider_->setMinValue(static_cast<int>((val-property_->getRangeMin())*maxNumberOfValues_));
    setPropertyValue(static_cast<float>(val), static_cast<float>(spinBoxMax_->value()));
}

void FloatMinMaxPropertyWidgetQt::updateFromSpinBoxMax(double val){
    slider_->setMaxValue(static_cast<int>((val-property_->getRangeMin())*maxNumberOfValues_));
    setPropertyValue(static_cast<float>(spinBoxMin_->value()), static_cast<float>(val));
}

void FloatMinMaxPropertyWidgetQt::setPropertyValue(float minVal, float maxVal) {
    property_->set(vec2(minVal, maxVal));
    emit modified();
}

void FloatMinMaxPropertyWidgetQt::setSpinBoxDecimals(float increment) {
    std::ostringstream buff;
    buff<<increment;

    std::string str2 = buff.str().substr(buff.str().find(".")+1,buff.str().length());

    spinBoxMin_->setDecimals(str2.length());
    spinBoxMax_->setDecimals(str2.length());
}

} //namespace