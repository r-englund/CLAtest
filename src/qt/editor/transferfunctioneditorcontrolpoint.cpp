#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <inviwo/qt/editor/transferfunctioneditorcontrolpoint.h>

namespace inviwo {

    TransferFunctionEditorControlPoint::TransferFunctionEditorControlPoint(QPointF* position){
        setFlag(QGraphicsItem::ItemIsMovable, true);
        setFlag(QGraphicsItem::ItemIsSelectable);
        setFlag(QGraphicsItem::ItemIsMovable);
        setFlag(QGraphicsItem::ItemSendsGeometryChanges);
        setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
        this->setPosition(new QPointF((*position)));
    }

    TransferFunctionEditorControlPoint::TransferFunctionEditorControlPoint(float x, float y){
        setFlag(QGraphicsItem::ItemIsMovable, true);
        setFlag(QGraphicsItem::ItemIsSelectable);
        setFlag(QGraphicsItem::ItemIsMovable);
        setFlag(QGraphicsItem::ItemSendsGeometryChanges);
        setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
        this->setPosition(new QPointF(x, y));
    }

    void TransferFunctionEditorControlPoint::setId(int id){
        id_ = id;
    }

    const int TransferFunctionEditorControlPoint::getId(){
        return id_;
    }

    TransferFunctionEditorControlPoint::TransferFunctionEditorControlPoint(){};

    TransferFunctionEditorControlPoint::~TransferFunctionEditorControlPoint() {};

    void TransferFunctionEditorControlPoint::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget* widget) {
        IVW_UNUSED_PARAM(options);
        IVW_UNUSED_PARAM(widget);

        painter->setRenderHint(QPainter::Antialiasing, true);

        float size = 10.0f;

        setFlag(QGraphicsItem::ItemIsMovable, true);

        QPen* pen = new QPen(Qt::black, 2.5, Qt::SolidLine, Qt::RoundCap);
        painter->setPen(*pen);
        painter->drawEllipse(this->getPosition()->x() - size/2, this->getPosition()->y() - size/2, size, size);

        pen = new QPen(Qt::cyan, 1.0, Qt::SolidLine, Qt::RoundCap);
        painter->setPen(*pen);
        painter->drawEllipse(pos_->x() - size/2, pos_->y() - size/2, size, size);

        //Draw the bounding rectangle
        if (false)
        {
	        float left = pos_->x() - size/2;
	        float top = pos_->y() - size/2;
	        painter->drawRect(left, top, size, size);
        }
    }

    QRectF TransferFunctionEditorControlPoint::boundingRect() const {
        float size = 12.0f;
        float left = pos_->x() - size/2;
        float top = pos_->y() - size/2;
        return QRectF(left, top, size, size);
    }

    void TransferFunctionEditorControlPoint::mousePressEvent ( QGraphicsSceneMouseEvent *e ){}

    void TransferFunctionEditorControlPoint::mouseReleaseEvent( QGraphicsSceneMouseEvent *e ){}

    void TransferFunctionEditorControlPoint::mouseMoveEvent(QGraphicsSceneMouseEvent * e)
    {
        QPointF* pos = new QPointF(e->scenePos());

        if (id_ == 0){
            pos->setX(0);
            pos->setY(e->scenePos().y());
        }
        if (id_ == 1){
            pos->setX(255);
            pos->setY(e->scenePos().y());
        }
        if (pos->x() <= 0){
            pos->setX(0);
        } 
        if (pos->x() >= 255){
            pos->setX(255);
        }
        if (pos->y() <= 0){
            pos->setY(0);
        } 
        if (pos->y() >= 100){
            pos->setY(100);
        }
        setPosition(pos);
    }

    void TransferFunctionEditorControlPoint::setPosition(QPointF* pos){
        pos_ =  new QPointF(pos->x(), pos->y());
    }

    const QPointF* TransferFunctionEditorControlPoint::getPosition(){
        return new QPointF(pos_->x(), pos_->y());
    }
} // namespace