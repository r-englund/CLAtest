#include <inviwo/qt/widgets/floatsliderwidgetqt.h>

namespace inviwo {
     
FloatSliderWidgetQt::FloatSliderWidgetQt() {
    generateWidget();
}

void FloatSliderWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    slider_ = new CustomSliderWidgetQt();
    slider_->setOrientation(Qt::Horizontal);
    slider_->setMaximum(SLIDER_MAX);
    spinBox_ = new QDoubleSpinBox();
    spinBox_->setFixedWidth(50);
    hLayout->addWidget(slider_);
    hLayout->addWidget(spinBox_);
    hLayout->setContentsMargins(0,0,0,0);
    hLayout->setSpacing(0);
    setLayout(hLayout);

    connect(slider_,  SIGNAL(valueChanged(int)),    this, SLOT(updateFromSlider()));
    connect(spinBox_, SIGNAL(valueChanged(double)), this, SLOT(updateFromSpinBox()));
}

FloatSliderWidgetQt::~FloatSliderWidgetQt() {}


void FloatSliderWidgetQt::setValue(float value) {
    if (value>=minValue_ && value<=maxValue_ && value!=value_) {
        value_ = value;
        emit valueChanged(value_);
    }
    updateSlider();
    updateSpinBox();
}

float inviwo::FloatSliderWidgetQt::getValue() {
    return value_;
}

void FloatSliderWidgetQt::setMinValue(float minValue) {
    minValue_ = minValue;
    spinBox_->setMinimum(static_cast<double>(minValue));
}

void FloatSliderWidgetQt::setMaxValue(float maxValue) {
    maxValue_ = maxValue;
    spinBox_->setMaximum(static_cast<double>(maxValue));
}

void FloatSliderWidgetQt::setRange(float minValue, float maxValue) {
    setMinValue(minValue);
    setMaxValue(maxValue);
}

void FloatSliderWidgetQt::setIncrement(float increment) {
    increment_ = increment;
    spinBox_->setSingleStep(static_cast<double>(increment));
    slider_->setSingleStep(10);
    setSpinBoxDecimals(increment);
}


void FloatSliderWidgetQt::updateFromSlider() {
    float normalizedValue = static_cast<float>(slider_->value())/static_cast<float>(slider_->maximum());
    float newValue = minValue_ + (normalizedValue * (maxValue_ - minValue_));
    if (newValue != value_) {
        value_ = newValue;
        updateSpinBox();
        emit valueChanged(value_);
    }
}

void FloatSliderWidgetQt::updateSlider() {
    float normalizedValue = (value_-minValue_)/(maxValue_-minValue_);
    int newValue = static_cast<int>(normalizedValue * SLIDER_MAX);
    slider_->blockSignals(true);
    slider_->setValue(newValue);
    slider_->blockSignals(false);
}


void FloatSliderWidgetQt::updateFromSpinBox() {
    float newValue = static_cast<float>(spinBox_->value());
    if (newValue != value_) {
        value_ = newValue;
        updateSlider();
        emit valueChanged(value_);
    }
}

void FloatSliderWidgetQt::updateSpinBox() {
    spinBox_->blockSignals(true);
    spinBox_->setValue(static_cast<double>(value_));
    spinBox_->blockSignals(false);
}

void FloatSliderWidgetQt::setSpinBoxDecimals( float increment ) {

    std::ostringstream buff;
    buff<<increment;

    std::string str2 = buff.str().substr(buff.str().find(".")+1,buff.str().length());

    spinBox_->setDecimals(str2.length());

}

} // namespace inviwo
