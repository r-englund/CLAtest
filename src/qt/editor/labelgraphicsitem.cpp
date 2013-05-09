#include <QFocusEvent>
#include <QFont>
#include <QPainter>

#include <inviwo/qt/editor/labelgraphicsitem.h>
#include <inviwo/qt/editor/processorgraphicsitem.h>

namespace inviwo {

LabelGraphicsItem::LabelGraphicsItem(QGraphicsItem* parent) : QGraphicsSimpleTextItem(parent) {
}

LabelGraphicsItem::~LabelGraphicsItem() {}

void LabelGraphicsItem::focusOutEvent(QFocusEvent* event) {
    setFlags(0);
    //setTextInteractionFlags(Qt::NoTextInteraction);
    //ProcessorGraphicsItem* processorGraphicsItem = dynamic_cast<ProcessorGraphicsItem*>(parent());
    //if (processorGraphicsItem) processorGraphicsItem->getProcessor()->setIdentifier(toPlainText().toLocal8Bit().constData());
}


} // namespace
