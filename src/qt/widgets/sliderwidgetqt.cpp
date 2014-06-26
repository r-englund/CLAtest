#include <inviwo/qt/widgets/sliderwidgetqt.h>
#include <limits>

namespace inviwo {

BaseSliderWidgetQt::BaseSliderWidgetQt()
    : QWidget(), spinBox_(NULL), slider_(NULL), spinnerValue_(0.0), sliderValue_(0) {
    generateWidget();
}

BaseSliderWidgetQt::~BaseSliderWidgetQt() {}

void BaseSliderWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    slider_ = new QSlider();
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

    QSizePolicy sp = sizePolicy();
    sp.setVerticalPolicy(QSizePolicy::Fixed);
    setSizePolicy(sp);
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
    spinBox_->blockSignals(true);
    spinBox_->setMinimum(transformMinValueToSpinner());
    spinBox_->blockSignals(false);
    slider_->blockSignals(true);
    slider_->setMinimum(transformMinValueToSlider());
    slider_->blockSignals(false);
    updateSlider();
}
void BaseSliderWidgetQt::applyMaxValue() {
    spinBox_->blockSignals(true);
    spinBox_->setMaximum(transformMaxValueToSpinner());
    spinBox_->blockSignals(false);
    slider_->blockSignals(true);
    slider_->setMaximum(transformMaxValueToSlider());
    slider_->blockSignals(false);
    updateSlider();
}
void BaseSliderWidgetQt::applyIncrement() {
    spinBox_->blockSignals(true);
    spinBox_->setSingleStep(transformIncrementToSpinner());
    spinBox_->setDecimals(transformIncrementToSpinnerDecimals());
    spinBox_->blockSignals(false);
    slider_->blockSignals(true);
    slider_->setSingleStep(transformIncrementToSlider());
    slider_->blockSignals(false);
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
    if(fabs(newValue - spinnerValue_) > std::numeric_limits<double>::epsilon()) {
        spinnerValue_ = newValue;
        newSpinnerValue(spinnerValue_);
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

