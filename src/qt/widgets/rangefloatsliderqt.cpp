#include <inviwo/qt/widgets/rangefloatsliderqt.h>
#include <QFrame>

namespace inviwo {
     
RangeFloatSliderQt::RangeFloatSliderQt(QWidget *parent) : QSplitter(Qt::Horizontal, parent) {
    QFrame *left = new QFrame();
    QFrame *middle = new QFrame();
    QFrame *right = new QFrame();
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
}

RangeFloatSliderQt::~RangeFloatSliderQt() {}

void RangeFloatSliderQt::resizeEvent(QResizeEvent *event) {
    QSplitter::resizeEvent(event);
}

int RangeFloatSliderQt::value() { 
    return 0;
}

void RangeFloatSliderQt::setValue(int value) { 
}

} // namespace inviwo
