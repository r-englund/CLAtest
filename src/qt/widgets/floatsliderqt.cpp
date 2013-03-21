#include <inviwo/qt/widgets/floatsliderqt.h>
#include <math.h>


namespace inviwo {
     
FloatSliderQt::FloatSliderQt( float minValue_, float maxValue_ ) : 
QSlider(Qt::Horizontal) {
    this->minValue_ = minValue_;
    this->maxValue_ = maxValue_;
}

inviwo::FloatSliderQt::~FloatSliderQt() {}

float inviwo::FloatSliderQt::getValue() {
    sliderValue_ = (maxValue_-minValue_)*(static_cast<float>(value())/maximum())+minValue_;
    return sliderValue_;
}

void FloatSliderQt::setValue( float tmpValue ) {
    if (minValue_<tmpValue<=maxValue_) {
    sliderValue_ =static_cast<int>(ceilf(((tmpValue-minValue_)/(maxValue_ - minValue_)) * (maximum()-minimum())));
    setSliderPosition(static_cast<int>(sliderValue_));
    }
}

void FloatSliderQt::setMinValue( float tmpValue ) {
    minValue_ = tmpValue;
}

void FloatSliderQt::setMaxValue( float tmpValue ) {
    maxValue_ = tmpValue;
}

void FloatSliderQt::setRange( float min,float max ) {
 minValue_ = min;
 maxValue_ = max;
}

//void FloatSliderQt::setOrientation( Qt::Orientation orientation_) {
//    setOrientation(orientation_);
//}

}//Namespace
