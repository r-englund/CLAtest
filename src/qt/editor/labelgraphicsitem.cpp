#include <QFocusEvent>
#include <QFont>
#include <QPainter>

#include <inviwo/qt/editor/labelgraphicsitem.h>
#include <inviwo/qt/editor/processorgraphicsitem.h>

namespace inviwo {

LabelGraphicsItem::LabelGraphicsItem(QGraphicsItem* parent) : QGraphicsTextItem(parent) {
}

LabelGraphicsItem::~LabelGraphicsItem() {}

QString LabelGraphicsItem::text() const{
    return toPlainText();
}

void LabelGraphicsItem::setText(const QString &str){
    setPlainText(str);
}

void LabelGraphicsItem::keyPressEvent(QKeyEvent *event){
    if (event->key() == Qt::Key_Return)
        clearFocus();
    else if (event->key() != Qt::Key_Space )
        QGraphicsTextItem::keyPressEvent(event);
}

void LabelGraphicsItem::focusOutEvent(QFocusEvent* event) {
    setFlags(0);
    setTextInteractionFlags(Qt::NoTextInteraction);
    ProcessorGraphicsItem* processorGraphicsItem = dynamic_cast<ProcessorGraphicsItem*>(this->parentItem());
    if (processorGraphicsItem) 
        processorGraphicsItem->getProcessor()->setIdentifier(toPlainText().toLocal8Bit().constData());

    if(text().length()>17){
        this->setToolTip(text());
        setText(text().left(8) + "..." + text().right(7));
    }
}

} // namespace
