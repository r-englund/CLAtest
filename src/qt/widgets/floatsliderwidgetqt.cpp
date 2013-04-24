#include <inviwo/qt/widgets/floatsliderwidgetqt.h>

namespace inviwo {
     
FloatSliderWidgetQt::FloatSliderWidgetQt(float minValue, float maxValue, float increment) {
    generateWidget();
    setMinValue(minValue);
    setMaxValue(minValue);
    setIncrement(increment);
}

void FloatSliderWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    slider_ = new QSlider(Qt::Horizontal);
    //slider_->setTickInterval(static_cast<int>(ceilf((maxValue_-minValue_)/increment_)));
    //slider_->setTickPosition(QSlider::TicksBothSides);
    //slider_->setTickInterval(10);
    //slider_->setSingleStep(1);
    //slider_->setMaximum(static_cast<int>(ceilf((maxValue_-minValue_)/increment_)+1000)); //FIXME: ???
    spinBox_ = new QDoubleSpinBox();
    hLayout->addWidget(slider_);
    hLayout->addWidget(spinBox_);
    setLayout(hLayout);

    connect(slider_,  SIGNAL(valueChanged(int)),    this, SLOT(updateFromSlider()));
    connect(spinBox_, SIGNAL(valueChanged(double)), this, SLOT(updateFromSpinBox()));
}

FloatSliderWidgetQt::~FloatSliderWidgetQt() {}


void FloatSliderWidgetQt::setValue(float value) {
    if (value>=minValue_ && value<=maxValue_ && value!=value_) {
        value_ = value;
        updateSlider();
        updateSpinBox();
        emit valueChanged(value_);
    }
}

float inviwo::FloatSliderWidgetQt::getValue() {
    return value_;
}


void FloatSliderWidgetQt::setMinValue(float minValue) {
    minValue_ = minValue;
    spinBox_->setMinimum(static_cast<double>(minValue));
    updateSliderRange();
}

void FloatSliderWidgetQt::setMaxValue(float maxValue) {
    maxValue_ = maxValue;
    spinBox_->setMaximum(static_cast<double>(maxValue));
    updateSliderRange();
}

void FloatSliderWidgetQt::setRange(float minValue, float maxValue) {
    setMinValue(minValue);
    setMaxValue(maxValue);
}


void FloatSliderWidgetQt::updateSliderRange() {
    int maxValue = static_cast<int>(ceilf((maxValue_-minValue_)/increment_));
    slider_->setMaximum(maxValue);
}


void FloatSliderWidgetQt::setIncrement(float increment) {
    increment_ = increment;
    updateDecimals();
    spinBox_->setSingleStep(static_cast<double>(increment));
    slider_->setSingleStep(10);
    updateSliderRange();
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
    int newValue = static_cast<int>(normalizedValue * slider_->maximum());
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


void FloatSliderWidgetQt::updateDecimals() {
    //Not working....
    float tmpIncrement = increment_;
    int count = 0;
    while(fmod(tmpIncrement,10.0f)!= 0) {
        tmpIncrement=tmpIncrement*10.0f;
        tmpIncrement = (int) (tmpIncrement * 100.0f) / 100.0f;
        ++count;
    }
    //std::cout << "Number of decimals:   " << count << std::endl;
    //spinBox_->setDecimals(count);
    spinBox_->setDecimals(4);
}

} // namespace inviwo
