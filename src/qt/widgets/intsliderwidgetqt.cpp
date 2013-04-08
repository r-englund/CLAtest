#include <inviwo/qt/widgets/intsliderwidgetqt.h>

namespace inviwo {
inviwo::IntSliderWidgetQt::IntSliderWidgetQt( int minValue_, int maxValue_, int increment_ ) {
    this->minValue_ = minValue_;
    this->maxValue_ = maxValue_;
    this->increment_= increment_;
    generateWidget();

}

void inviwo::IntSliderWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    slider_ = new QSlider(Qt::Horizontal);

    //slider_->setMaximum((maxValue_-minValue_)/increment_);
    slider_->setRange(this->minValue_,this->maxValue_);
    spinBox_ = new QSpinBox();
    spinBox_->setRange(this->minValue_,this->maxValue_);
    spinBox_->setSingleStep(this->increment_);

    hLayout->addWidget(slider_);
    hLayout->addWidget(spinBox_);
    setLayout(hLayout);

}

inviwo::IntSliderWidgetQt::~IntSliderWidgetQt(){}

int inviwo::IntSliderWidgetQt::getValue() {
return slider_->value();

}

void inviwo::IntSliderWidgetQt::setValue( int tmpValue ) {
    if (minValue_<tmpValue || tmpValue<=maxValue_) {
    slider_->setValue(tmpValue);
    spinBox_->setValue(tmpValue);
    }

}

void inviwo::IntSliderWidgetQt::setMaxValue( int max ) {
    maxValue_ = max;
    spinBox_->setMaximum(maxValue_);
}

void inviwo::IntSliderWidgetQt::setMinValue( int min ) {
    minValue_ = min;
    spinBox_->setMinimum(minValue_);
}

void inviwo::IntSliderWidgetQt::setRange( int min,int max ) {
    setMinValue(min);
    setMaxValue(max);
}

QSpinBox* inviwo::IntSliderWidgetQt::getSpinBox() {
    return spinBox_;

}

QSlider* inviwo::IntSliderWidgetQt::getSlider() {
    return slider_;

}

void inviwo::IntSliderWidgetQt::updateValueSpinBox() {
    if (spinBox_->value()!=slider_->value()) {
        spinBox_->setValue(slider_->value());
    }

}

void inviwo::IntSliderWidgetQt::updateValueSlider() {
    if (spinBox_->value()!=slider_->value()) {
        slider_->setValue(spinBox_->value());
    }

}

void IntSliderWidgetQt::setIncrement( int increment ) {
 this->increment_ = increment_;
 spinBox_->setSingleStep(increment_);
 //slider_->setMaximum((maxValue_-minValue_)/increment_);
     slider_->setRange(this->minValue_,this->maxValue_);
}

}//namespace