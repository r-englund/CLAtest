#ifndef IVW_SLIDETWIDGETQT_H
#define IVW_SLIDETWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/customdoublespinboxqt.h>

#include <QSlider>
#include <QHBoxLayout>
#include <math.h>
#include <sstream>

namespace inviwo {

class IVW_QTWIDGETS_API BaseSliderWidgetQt : public QWidget {
    Q_OBJECT
public:
    BaseSliderWidgetQt();
    virtual ~BaseSliderWidgetQt();

protected: 
    virtual double transformValueToSpinner() = 0;
    virtual int transformValueToSlider() = 0;

    virtual double transformMinValueToSpinner() = 0;
    virtual int transformMinValueToSlider() = 0;

    virtual double transformMaxValueToSpinner() = 0;
    virtual int transformMaxValueToSlider() = 0;

    virtual double transformIncrementToSpinner() = 0;
    virtual int transformIncrementToSpinnerDecimals() = 0;
    virtual int transformIncrementToSlider() = 0;

    virtual void newSliderValue(int val) = 0;
    virtual void newSpinnerValue(double val) = 0;

    void applyInit();
    void applyValue();
    void applyMinValue();
    void applyMaxValue();
    void applyIncrement();

private slots:
    void updateFromSlider();
    void updateFromSpinBox();

signals:
    void valueChanged();

private:
    void generateWidget();
    /**
    * \brief updates the value of the spin box from the slider value
    */
    void updateSpinBox();
    /**
    * \brief updates the value of the slider from the spin box value
    */
    void updateSlider();

    static const int SLIDER_MAX = 10000;
    CustomDoubleSpinBoxQt* spinBox_;
    QSlider* slider_;
    double spinnerValue_;
    int sliderValue_;

};

template <typename T>
class TemplateSliderWidget : public BaseSliderWidgetQt {
public:
    TemplateSliderWidget() : BaseSliderWidgetQt() {}
    virtual ~TemplateSliderWidget() {}

    T getValue();
    void setValue(T value);
    void initValue(T value);
    void setMinValue(T minValue);
    void setMaxValue(T maxValue);
    void setRange(T minValue, T maxValue);
    void setIncrement(T increment);

protected:
    // Define the transforms
    virtual T sliderToRepr(int val) = 0;
    virtual int reprToSlider(T val) = 0;
    virtual T spinnerToRepr(double val) = 0;
    virtual double reprToSpinner(T val) = 0;

    // Has default implementations using above transformations.
    virtual double transformValueToSpinner();
    virtual int transformValueToSlider();

    virtual double transformMinValueToSpinner();
    virtual int transformMinValueToSlider();

    virtual double transformMaxValueToSpinner();
    virtual int transformMaxValueToSlider();

    virtual double transformIncrementToSpinner();
    virtual int transformIncrementToSpinnerDecimals();
    virtual int transformIncrementToSlider();

    virtual void newSliderValue(int val);
    virtual void newSpinnerValue(double val);

    T value_;
    T minValue_;
    T maxValue_;
    T increment_;
};

// Default case for fractional numbers
template <typename T>
class SliderWidgetQt : public TemplateSliderWidget<T> {
public:
    SliderWidgetQt() : TemplateSliderWidget<T>() {}
    virtual ~SliderWidgetQt() {}

protected:  
    // Defines the transform
    virtual T sliderToRepr(int val) {
        return this->minValue_ + (static_cast<T>(val) * (this->maxValue_ - this->minValue_) / static_cast<T>(SLIDER_MAX));
    }
    virtual int reprToSlider(T val) {
        return static_cast<int>((val - this->minValue_) / (this->maxValue_ - this->minValue_) * SLIDER_MAX);
    } 
    virtual T spinnerToRepr(double val) {
        return static_cast<T>(val);
    }
    virtual double reprToSpinner(T val) {
        return static_cast<double>(val);
    }

    // Custom min and max for the slider
    virtual int transformMinValueToSlider() {
        return 0;
    }
    virtual int transformMaxValueToSlider() {
        return SLIDER_MAX;
    };

private:
    static const int SLIDER_MAX = 10000;
};

// Specialization for integer types
template <>
class SliderWidgetQt<int> : public TemplateSliderWidget<int>{
public:
    SliderWidgetQt() : TemplateSliderWidget<int>() {}
    virtual ~SliderWidgetQt() {}

protected:
    // Defines the transform
    virtual int sliderToRepr(int val) {
        return val;
    }
    virtual int reprToSlider(int val) {
        return val;
    }
    virtual int spinnerToRepr(double val) {
        return static_cast<int>(val);
    }
    virtual double reprToSpinner(int val) {
        return static_cast<double>(val);
    }
    virtual int transformIncrementToSpinnerDecimals() {
        return 0;
    }
};

typedef SliderWidgetQt<int> IntSliderWidgetQt;
typedef SliderWidgetQt<float> FloatSliderWidgetQt;
typedef SliderWidgetQt<double> DoubleSliderWidgetQt;

template <typename T>
double inviwo::TemplateSliderWidget<T>::transformValueToSpinner() {
    return reprToSpinner(value_);
}

template <typename T>
int inviwo::TemplateSliderWidget<T>::transformValueToSlider() {
    return reprToSlider(value_);
}

template <typename T>
double inviwo::TemplateSliderWidget<T>::transformMinValueToSpinner() {
    return reprToSpinner(minValue_);
}

template <typename T>
int inviwo::TemplateSliderWidget<T>::transformMinValueToSlider() {
    return reprToSlider(minValue_);
}

template <typename T>
double inviwo::TemplateSliderWidget<T>::transformMaxValueToSpinner() {
    return reprToSpinner(maxValue_);
}

template <typename T>
int inviwo::TemplateSliderWidget<T>::transformMaxValueToSlider() {
    return reprToSlider(maxValue_);
}

template <typename T>
double inviwo::TemplateSliderWidget<T>::transformIncrementToSpinner() {
    return reprToSpinner(increment_);
}

template <typename T>
int inviwo::TemplateSliderWidget<T>::transformIncrementToSpinnerDecimals() {
    double inc = reprToSpinner(increment_);
    std::ostringstream buff;
    buff << inc;
    std::string str2 = buff.str().substr(buff.str().find(".") + 1, buff.str().length());
    return (static_cast<int>(str2.length()));
}

template <typename T>
int inviwo::TemplateSliderWidget<T>::transformIncrementToSlider() {
    return reprToSlider(increment_);
}

template <typename T>
void inviwo::TemplateSliderWidget<T>::newSpinnerValue(double val) {
    value_ = spinnerToRepr(val);
}

template <typename T>
void inviwo::TemplateSliderWidget<T>::newSliderValue(int val) {
    value_ = sliderToRepr(val);
}

template <typename T>
T TemplateSliderWidget<T>::getValue() {
    return value_;
}

template <typename T>
void TemplateSliderWidget<T>::setValue(T value) {
    if(value >= minValue_ && value <= maxValue_ && value != value_) {
        value_ = value;
        applyValue();
    }
}

template <typename T>
void TemplateSliderWidget<T>::initValue(T value) {
    value_ = value;
    applyInit();
}

template <typename T>
void TemplateSliderWidget<T>::setMinValue(T minValue) {
    if(minValue_ != minValue) {
        minValue_ = minValue;
        applyMinValue();
    }
}

template <typename T>
void TemplateSliderWidget<T>::setMaxValue(T maxValue) {
    if(maxValue_ != maxValue) {
        maxValue_ = maxValue;
        applyMaxValue();
    }
}

template <typename T>
void TemplateSliderWidget<T>::setRange(T minValue, T maxValue) {
    setMinValue(minValue);
    setMaxValue(maxValue);
}

template <typename T>
void TemplateSliderWidget<T>::setIncrement(T increment) {
    if(increment_ != increment) {
        increment_ = increment;
        applyIncrement();
    }
}




} // namespace

#endif // IVW_SLIDETWIDGETQT_H

