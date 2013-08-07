#include <QFocusEvent>
#include <QFont>
#include <QPainter>
#include <QTextCursor>

#include <inviwo/qt/editor/labelgraphicsitem.h>
#include <inviwo/qt/editor/processorgraphicsitem.h>

namespace inviwo {

LabelGraphicsItem::LabelGraphicsItem(QGraphicsItem* parent) : QGraphicsTextItem(parent), orgText_(""), maxBefore_(0), maxAfter_(0) {
}

LabelGraphicsItem::~LabelGraphicsItem() {}

QString LabelGraphicsItem::text() const{
    if(isCropped())
        return orgText_;
    else
        return toPlainText();
}

void LabelGraphicsItem::setText(const QString &str){
    if(doCrop(str)){
        if(toolTip()==orgText_)
            setToolTip(str);
        orgText_ = str;
        setPlainText(str.left(maxBefore_) + "..." + str.right(maxAfter_));
    }
    else{
        orgText_="";
        setToolTip(orgText_);
        setPlainText(str);
    }
}

QString LabelGraphicsItem::croppedText() const{
    return toPlainText();
}

void LabelGraphicsItem::setCrop(int before, int after){
    maxBefore_ = before;
    maxAfter_ = after;
    updateCrop();
}

bool LabelGraphicsItem::isCropped() const{
    return (!orgText_.isEmpty());
}

bool LabelGraphicsItem::doCrop(const QString &str){
    return (maxBefore_ + maxAfter_ + 2 < str.length());
}

void LabelGraphicsItem::updateCrop(){
    setText(toPlainText());
}

void LabelGraphicsItem::updateGraphicsItem(){
    ProcessorGraphicsItem* processorGraphicsItem = dynamic_cast<ProcessorGraphicsItem*>(this->parentItem());
    if (processorGraphicsItem){ 
        processorGraphicsItem->getProcessor()->setIdentifier(text().toLocal8Bit().constData());
        processorGraphicsItem->updatePropertyListWidget();
    }
}

void LabelGraphicsItem::keyPressEvent(QKeyEvent *event){
    if (event->key() == Qt::Key_Return)
        clearFocus();
    else if (event->key() != Qt::Key_Space )
        QGraphicsTextItem::keyPressEvent(event);
}

void LabelGraphicsItem::focusInEvent(QFocusEvent* event) {
    if(isCropped())
        setPlainText(orgText_);
}

void LabelGraphicsItem::focusOutEvent(QFocusEvent* event) {
    setFlags(0);
    setTextInteractionFlags(Qt::NoTextInteraction);
    QTextCursor cur = QTextCursor(textCursor());
    cur.clearSelection();
    setTextCursor(cur);
    updateCrop();
    updateGraphicsItem();
}

} // namespace
