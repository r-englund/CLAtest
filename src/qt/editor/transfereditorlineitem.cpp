#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <inviwo/qt/editor/transfereditorlineitem.h>

namespace inviwo {

    const std::string TransferEditorLineItem::logSource_ = "TransferEditorLineItem";

    TransferEditorLineItem::TransferEditorLineItem(TransferEditorControlPoint* start_, TransferEditorControlPoint* finish_)
    {
        start = start_;
        finish = finish_;
        startPos = start->position();
    };

    TransferEditorLineItem::TransferEditorLineItem(){};

    TransferEditorLineItem::~TransferEditorLineItem() {};


    void TransferEditorLineItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget* widget) {
        IVW_UNUSED_PARAM(options);
        IVW_UNUSED_PARAM(widget);

        QPointF point1;
        QPointF point2;

        point1 = start->pos();
        point2 = finish->pos();

        painter->setRenderHint(QPainter::Antialiasing, true);

        QPen* pen = new QPen(Qt::black, 2.5, Qt::SolidLine, Qt::RoundCap);
        painter->setPen(*pen);
        painter->drawLine(point1 * 2, point2 * 2);

        pen = new QPen(Qt::cyan, 1.0, Qt::SolidLine, Qt::RoundCap);
        painter->setPen(*pen);
        painter->drawLine(point1 * 2, point2 * 2);
    }


    QRectF TransferEditorLineItem::boundingRect() const {
        return QRectF(start->pos(), finish->pos());
    }
    void TransferEditorLineItem::mousePressEvent ( QGraphicsSceneMouseEvent *e ){}

    void TransferEditorLineItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *e ){}

    void TransferEditorLineItem::mouseMoveEvent(QGraphicsSceneMouseEvent * e){}

    QPointF TransferEditorLineItem::getStart(){return this->start->position();}

    QPointF TransferEditorLineItem::getFinish(){return this->finish->position();}

    void TransferEditorLineItem::setStart(TransferEditorControlPoint* start_){
        start = start_;
        startPos = start->position();
    }
    void TransferEditorLineItem::setFinish(TransferEditorControlPoint* finish_){
        finish = finish_;
    }
} // namespace