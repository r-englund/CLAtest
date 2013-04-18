#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <inviwo/qt/editor/transferfunctioneditorlineitem.h>

namespace inviwo {

    TransferFunctionEditorLineItem::TransferFunctionEditorLineItem(TransferFunctionEditorControlPoint* start, TransferFunctionEditorControlPoint* finish)
    {
        start_ = start;
        finish_ = finish;
    };

    TransferFunctionEditorLineItem::TransferFunctionEditorLineItem(){};

    TransferFunctionEditorLineItem::~TransferFunctionEditorLineItem() {};


    void TransferFunctionEditorLineItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget* widget) {
        IVW_UNUSED_PARAM(options);
        IVW_UNUSED_PARAM(widget);
        const QPointF* point1 = start_->getPosition();
        const QPointF* point2 = finish_->getPosition();

        painter->setRenderHint(QPainter::Antialiasing, true);

        QPen* pen = new QPen(Qt::black, 2.5, Qt::SolidLine, Qt::RoundCap);
        painter->setPen(*pen);
        painter->drawLine((*point1), (*point2));

        pen = new QPen(Qt::cyan, 1.0, Qt::SolidLine, Qt::RoundCap);
        painter->setPen(*pen);
        painter->drawLine((*point1), (*point2));
    }


    QRectF TransferFunctionEditorLineItem::boundingRect() const {
        return QRectF(start_->getPosition()->x(), start_->getPosition()->y(), finish_->getPosition()->x(), finish_->getPosition()->y());
    }
    void TransferFunctionEditorLineItem::mousePressEvent ( QGraphicsSceneMouseEvent *e ){}

    void TransferFunctionEditorLineItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *e ){}

    void TransferFunctionEditorLineItem::mouseMoveEvent(QGraphicsSceneMouseEvent * e){}

    const QPointF* TransferFunctionEditorLineItem::getStart(){return this->start_->getPosition();}

    const QPointF* TransferFunctionEditorLineItem::getFinish(){return this->finish_->getPosition();}

    void TransferFunctionEditorLineItem::setStart(TransferFunctionEditorControlPoint* start){start_ = start;}
    void TransferFunctionEditorLineItem::setFinish(TransferFunctionEditorControlPoint* finish){finish_ = finish;}
} // namespace