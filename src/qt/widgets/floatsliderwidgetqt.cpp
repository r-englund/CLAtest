#include <inviwo/qt/widgets/floatsliderwidgetqt.h>
#include <iostream>



namespace inviwo {
     
FloatSliderWidgetQt::FloatSliderWidgetQt( float minValue_, float maxValue_, float increment_ ) {
    this->minValue_ = minValue_;
    this->maxValue_ = maxValue_;
    this->increment_= increment_;
    generateWidget();
}

void FloatSliderWidgetQt::generateWidget() {
    
    QHBoxLayout* hLayout = new QHBoxLayout();
    slider_ = new QSlider(Qt::Horizontal);
    /*slider_->setTickInterval(static_cast<int>(ceilf((maxValue_-minValue_)/increment_)));
    slider_->setTickPosition(QSlider::TicksBothSides);
    slider_->setTickInterval(10);
    slider_->setSingleStep(1);*/
    slider_->setMaximum(static_cast<int>(ceilf((maxValue_-minValue_)/increment_)+1000));
    spinBox_ = new QDoubleSpinBox();
    spinBox_->setRange(static_cast<double>(this->minValue_),static_cast<double>(this->maxValue_));
    spinBox_->setSingleStep(static_cast<double>(this->increment_));

    setDecimals();

    hLayout->addWidget(slider_);
    hLayout->addWidget(spinBox_);
    setLayout(hLayout);
}

inviwo::FloatSliderWidgetQt::~FloatSliderWidgetQt() {}

float inviwo::FloatSliderWidgetQt::getValue() {
    sliderValue_ = (maxValue_-minValue_)*(static_cast<float>(slider_->value())/slider_->maximum())+minValue_;
    return sliderValue_;
}

void FloatSliderWidgetQt::setValue( float tmpValue ) {
    if (minValue_<tmpValue || tmpValue<=maxValue_ && tmpValue != getValue()) {
    sliderValue_ =static_cast<int>(ceilf(((tmpValue-minValue_)/(maxValue_ - minValue_)) * (slider_->maximum()-slider_->minimum())));
    slider_->setSliderPosition(static_cast<int>(sliderValue_));
    }
}

void FloatSliderWidgetQt::setMinValue( float min ) {
    minValue_ = min;
    spinBox_->setMinimum(static_cast<double>(min));
}

void FloatSliderWidgetQt::setMaxValue( float max ) {
    maxValue_ = max;
    spinBox_->setMaximum(static_cast<double>(max));
}

void FloatSliderWidgetQt::setRange( float min,float max ) {
    setMinValue(min);
    setMaxValue(max);
}

QDoubleSpinBox* FloatSliderWidgetQt::getSpinBox() {
    return spinBox_;
}

void FloatSliderWidgetQt::updateValueSpinBox() {

    if (static_cast<double>(getValue()) != spinBox_->value()) {
        spinBox_->setValue(static_cast<double>(getValue()));
    }
}

void FloatSliderWidgetQt::updateValueSlider() {

    if (getValue() != static_cast<float>(spinBox_->value())) {
        setValue(static_cast<float>(spinBox_->value()));
    }

}


QSlider* FloatSliderWidgetQt::getSlider() {
    return slider_;
}

void FloatSliderWidgetQt::setIncrement( float increment_ ) {

    this->increment_ = increment_;

    setDecimals();
    spinBox_->setSingleStep(static_cast<double>(this->increment_));
    slider_->setMaximum(static_cast<int>(ceilf((maxValue_-minValue_)/increment_)+1000));
    

}

void FloatSliderWidgetQt::setDecimals() {
        //Not working....
    float tmpIncrement = increment_;
    int count = 0;
    while(fmod(tmpIncrement,10.0f)!= 0) {
            tmpIncrement=tmpIncrement*10.0f;
            ++count;
        }
        //std::cout << "Number of decimals:   " << count << std::endl;
        //spinBox_->setDecimals(count);
        spinBox_->setDecimals(4);

}

}//Namespace
