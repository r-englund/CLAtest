#include <inviwo/qt/widgets/sliderwidgetqt.h>

namespace inviwo {

BaseSliderWidgetQt::BaseSliderWidgetQt() {
    generateWidget();
}

BaseSliderWidgetQt::~BaseSliderWidgetQt() {}

void BaseSliderWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    slider_ = new CustomSliderWidgetQt();
    slider_->setOrientation(Qt::Horizontal);
    slider_->setPageStep(1);
    slider_->setMaximum(SLIDER_MAX);
    spinBox_ = new CustomDoubleSpinBoxQt();
    spinBox_->setKeyboardTracking(false); // don't emit the valueChanged() signal while typing

    hLayout->addWidget(slider_);
    hLayout->addWidget(spinBox_);
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->setSpacing(0);
    setLayout(hLayout);
    connect(slider_, SIGNAL(valueChanged(int)), this, SLOT(updateFromSlider()));
    connect(spinBox_, SIGNAL(valueChanged(double)), this, SLOT(updateFromSpinBox()));
}

void BaseSliderWidgetQt::applyInit() {
    updateSlider();
    updateSpinBox();
}

void BaseSliderWidgetQt::applyValue() {
    applyInit();
    emit valueChanged();
}
void BaseSliderWidgetQt::applyMinValue() {
    spinBox_->setMinimum(transformMinValueToSpinner());
    slider_->setMinimum(transformMinValueToSlider());
    updateSlider();
}
void BaseSliderWidgetQt::applyMaxValue() {
    spinBox_->setMaximum(transformMaxValueToSpinner());
    slider_->setMaximum(transformMaxValueToSlider());
    updateSlider();
}
void BaseSliderWidgetQt::applyIncrement() {
    spinBox_->setSingleStep(transformIncrementToSpinner());
    spinBox_->setDecimals(transformIncrementToSpinnerDecimals());
    slider_->setSingleStep(transformIncrementToSlider());
}

void BaseSliderWidgetQt::updateFromSlider() {
    int newValue = slider_->value();
    if(newValue != sliderValue_) {
        sliderValue_ = newValue;
        newSliderValue(sliderValue_);
        updateSpinBox();
        emit valueChanged();
    }
}

void BaseSliderWidgetQt::updateFromSpinBox() {
    double newValue = spinBox_->value();
    if (fabs(newValue - sliderValue_) > std::numeric_limits<double>::epsilon()) {
        sliderValue_ = newValue;
        newSpinnerValue(sliderValue_);
        updateSlider();
        emit valueChanged();
    }
}

void BaseSliderWidgetQt::updateSpinBox() {
    spinBox_->blockSignals(true);
    spinBox_->setValue(transformValueToSpinner());
    spinBox_->blockSignals(false);
}

void BaseSliderWidgetQt::updateSlider() {
    slider_->blockSignals(true);
    slider_->setValue(transformValueToSlider());
    slider_->blockSignals(false);
}

} // namespace

