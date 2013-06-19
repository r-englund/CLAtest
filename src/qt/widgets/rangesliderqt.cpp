#include <inviwo/qt/widgets/rangesliderqt.h>
#include <QFrame>

namespace inviwo {
     
RangeSliderQt::RangeSliderQt(Qt::Orientation orientation, QWidget *parent) : QSplitter(orientation, parent) {
    QFrame *left = new QFrame(this);
    QFrame *middle = new QFrame(this);
    QFrame *right = new QFrame(this);
    addWidget(left);
    addWidget(middle);
    addWidget(right);

    left->setStyleSheet(QString(
        "QSplitter QFrame {"
        "  border-top-right-radius: 0px;"
        "  border-bottom-right-radius: 0px;"
        "}"
        ));
    middle->setObjectName("valueArea");
    right->setStyleSheet(QString(
        "QSplitter QFrame {"
        "  border-top-left-radius: 0px;"
        "  border-bottom-left-radius: 0px;"
        "}"
        ));

    if(orientation == Qt::Horizontal)
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    else
        setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);

    connect(this, SIGNAL(splitterMoved(int, int)), this, SLOT(updateSplitterPosition(int, int)));

    getRange(1, &range_[0], &value_[1]);
    getRange(2, &value_[0], &range_[1]);
}

RangeSliderQt::~RangeSliderQt() {}

int RangeSliderQt::minValue(){
    return value_[0];
}

int RangeSliderQt::maxValue(){
    return value_[1];
}

int RangeSliderQt::minRange(){
    return range_[0];
}

int RangeSliderQt::maxRange(){
    return range_[1];
}

void RangeSliderQt::setValue(int minVal, int maxVal){
    setMinValue(minVal);
    setMaxValue(maxVal);
}

void RangeSliderQt::setMinValue(int minVal){
    value_[0] = minVal;
}

void RangeSliderQt::setMaxValue(int maxVal){
    value_[1] = maxVal;
}

void RangeSliderQt::setRange(int minR, int maxR){
    setMinRange(minR);
    setMaxRange(maxR);
}

void RangeSliderQt::setMinRange(int minR){
    range_[0] = minR;
}

void RangeSliderQt::setMaxRange(int maxR){
    range_[1] = maxR;
}

//Index 1 = Min Handle, Index 2 = Max Handle
void RangeSliderQt::calculateInternalToExternalConversion(){
    getRange(1, &internalRange_[0], &internalValue_[1]);
    getRange(2, &internalValue_[0], &internalRange_[1]);

    fromInternalToExternal_ = (internalRange_[1]-internalRange_[0])/(range_[1]-range_[0]);
}

void RangeSliderQt::resizeEvent(QResizeEvent *event) {
    QSplitter::resizeEvent(event);
    getRange(1, &internalRange_[0], &internalValue_[1]);
    getRange(2, &internalValue_[0], &internalRange_[1]);
}

void RangeSliderQt::updateSplitterPosition(int pos, int idx){
    internalValue_[idx-1] = pos;
    value_[idx-1] = pos*((internalRange_[1]-internalRange_[0])/(range_[1]-range_[0]));
}

} // namespace inviwo
