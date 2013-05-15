#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <inviwo/qt/editor/transferfunctioneditorlineitem.h>

namespace inviwo {

    TransferFunctionEditorLineItem::TransferFunctionEditorLineItem(TransferFunctionDataPoint* start, TransferFunctionDataPoint* finish)
    {
        start_ = start;
        finish_ = finish;
        setZValue(0);
    };

    TransferFunctionEditorLineItem::TransferFunctionEditorLineItem(){};

    TransferFunctionEditorLineItem::~TransferFunctionEditorLineItem() {};


    void TransferFunctionEditorLineItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget* widget) {
        IVW_UNUSED_PARAM(options);
        IVW_UNUSED_PARAM(widget);
        const vec2* start = start_->getPos();
        const vec2* finish = finish_->getPos();

        painter->setRenderHint(QPainter::Antialiasing, true);

        QPen* pen = new QPen(Qt::black, 2.5, Qt::SolidLine, Qt::RoundCap);
        painter->setPen(*pen);
        painter->drawLine(start->x, start->y, finish->x, finish->y);

        pen = new QPen(Qt::cyan, 1.0, Qt::SolidLine, Qt::RoundCap);
        painter->setPen(*pen);
        painter->drawLine(start->x, start->y, finish->x, finish->y);
    }


    QRectF TransferFunctionEditorLineItem::boundingRect() const {
        return QRectF(0,0,0,0);
    }
    void TransferFunctionEditorLineItem::mousePressEvent ( QGraphicsSceneMouseEvent *e ){}

    void TransferFunctionEditorLineItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *e ){}

    void TransferFunctionEditorLineItem::mouseMoveEvent(QGraphicsSceneMouseEvent * e){}

    const QPointF* TransferFunctionEditorLineItem::getStart(){
        return new QPointF(start_->getPos()->x,start_->getPos()->y);
    }

    const QPointF* TransferFunctionEditorLineItem::getFinish(){
        return new QPointF(start_->getPos()->x,start_->getPos()->y);
    }

    void TransferFunctionEditorLineItem::setStart(TransferFunctionDataPoint* start){start_ = start;}
    void TransferFunctionEditorLineItem::setFinish(TransferFunctionDataPoint* finish){finish_ = finish;}
} // namespace