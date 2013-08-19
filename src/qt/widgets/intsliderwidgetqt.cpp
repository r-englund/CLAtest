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
    slider_ = new CustomSliderWidgetQt();
    slider_->setOrientation(Qt::Horizontal);
    slider_->setMaximum(SLIDER_MAX);
    spinBox_ = new QSpinBox();
    spinBox_->setRange(this->minValue_,this->maxValue_);
    spinBox_->setSingleStep(this->increment_);

    hLayout->addWidget(slider_);
    hLayout->addWidget(spinBox_);
    hLayout->setContentsMargins(0,0,0,0);
    hLayout->setSpacing(0);
    setLayout(hLayout);

    connect(slider_,  SIGNAL(valueChanged(int)),this, SLOT(updateFromSlider()));
    connect(spinBox_, SIGNAL(valueChanged(int)), this, SLOT(updateFromSpinBox()));

}

inviwo::IntSliderWidgetQt::~IntSliderWidgetQt(){}

int inviwo::IntSliderWidgetQt::getValue() {
    return value_;
}

void inviwo::IntSliderWidgetQt::setValue( int tmpValue ) {
    if (minValue_<tmpValue || tmpValue<=maxValue_) {
        value_ = tmpValue;
        updateValueSlider();
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
    if (spinBox_->value()!=value_) {
        spinBox_->setValue(value_);
    }

}

void inviwo::IntSliderWidgetQt::updateValueSlider() {
    float normalizedValue = float(float(value_-minValue_)/float(maxValue_-minValue_));
    int newValue = static_cast<int>(floor(normalizedValue * SLIDER_MAX));
    slider_->blockSignals(true);
    slider_->setValue(newValue);
    slider_->blockSignals(false);

}

void IntSliderWidgetQt::setIncrement( int increment ) {
 this->increment_ = increment_;
 spinBox_->setSingleStep(increment_);
}

void IntSliderWidgetQt::updateFromSpinBox(){
    int newValue = spinBox_->value();
    if (newValue != value_) {
        value_ = newValue;
        updateValueSlider();
        emit valueChanged(value_);
    }
}

void IntSliderWidgetQt::updateFromSlider(){
    float normalizedValue = static_cast<float>(slider_->value())/static_cast<float>(slider_->maximum());
    int newValue = static_cast<int>(floor(minValue_ + (normalizedValue * (maxValue_ - minValue_))));
    if (newValue != value_) {
        value_ = newValue;
        spinBox_->setValue(value_);
        emit valueChanged(value_);
    }

}

}//namespace