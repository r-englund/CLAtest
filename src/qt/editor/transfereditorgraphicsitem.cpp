#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <inviwo/qt/editor/transfereditorcontrolpoint.h>

namespace inviwo {

    const std::string TransferEditorControlPoint::logSource_ = "TransferEditorControlPoint";

    TransferEditorControlPoint::TransferEditorControlPoint(QPointF position){
        setFlag(QGraphicsItem::ItemIsMovable, true);
        setFlag(QGraphicsItem::ItemIsSelectable);
        setFlag(QGraphicsItem::ItemIsMovable);
        setFlag(QGraphicsItem::ItemSendsGeometryChanges);
        setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
        this->setPos(position/2);
    }

    TransferEditorControlPoint::TransferEditorControlPoint(float x, float y){
        setFlag(QGraphicsItem::ItemIsMovable, true);
        setFlag(QGraphicsItem::ItemIsSelectable);
        setFlag(QGraphicsItem::ItemIsMovable);
        setFlag(QGraphicsItem::ItemSendsGeometryChanges);
        setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
        this->setPos(x / 2, y / 2);
    }


    void TransferEditorControlPoint::setId(int id_){id = id_;}

    int TransferEditorControlPoint::getId(){return id;}

    TransferEditorControlPoint::TransferEditorControlPoint(){};


    TransferEditorControlPoint::~TransferEditorControlPoint() {}


    void TransferEditorControlPoint::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget* widget) {
        IVW_UNUSED_PARAM(options);
        IVW_UNUSED_PARAM(widget);

        QPointF position;
        position = this->scenePos();

        int size = 6;

        setFlag(QGraphicsItem::ItemIsMovable, true);
        painter->setRenderHint(QPainter::Antialiasing, true);

        QPen* pen = new QPen(Qt::black, 2.5, Qt::SolidLine, Qt::RoundCap);
        painter->setPen(*pen);
        painter->drawEllipse(position.x() - size/2, position.y() - size/2, size, size);

        pen = new QPen(Qt::cyan, 1.0, Qt::SolidLine, Qt::RoundCap);
        painter->setPen(*pen);
        painter->drawEllipse(position.x() - size/2, position.y() - size/2, size, size);
    }


    QRectF TransferEditorControlPoint::boundingRect() const {
        return QRectF(this->pos().x() - 8, this->pos().y() - 8, 16, 16);
    }

    void TransferEditorControlPoint::mousePressEvent ( QGraphicsSceneMouseEvent *e ){}

    void TransferEditorControlPoint::mouseReleaseEvent( QGraphicsSceneMouseEvent *e ){}

    void TransferEditorControlPoint::mouseMoveEvent(QGraphicsSceneMouseEvent * e)
    {
        QPointF position = e->scenePos();

        if (id == 0){
            position = QPointF(0, position.y());
        }
        if (id == 1){
            position = QPointF(255, position.y());
        }
        if (position.x() < 0){
            position = QPointF(0, position.y());
        } 
        if (position.x() > 255){
            position = QPointF(255, position.y());
        }
        if (position.y() < 0){
            position = QPointF(position.x(), 0);
        } 
        if (position.y() > 100){
            position = QPointF(position.x(), 100);
        }
        setPos(position/2);
    }

    QPointF TransferEditorControlPoint::position(){
        return this->pos() * 2;
    }
} // namespace