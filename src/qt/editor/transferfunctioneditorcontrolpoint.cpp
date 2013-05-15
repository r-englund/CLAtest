#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <inviwo/qt/editor/transferfunctioneditorcontrolpoint.h>

namespace inviwo {

    TransferFunctionEditorControlPoint::TransferFunctionEditorControlPoint(TransferFunctionDataPoint* datapoint):datapoint_(datapoint){
        setFlag(QGraphicsItem::ItemIsMovable, true);
        setFlag(QGraphicsItem::ItemIsSelectable);
        setFlag(QGraphicsItem::ItemIsMovable);
        setFlag(QGraphicsItem::ItemSendsGeometryChanges);
        setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
        setPos(datapoint_->getPos()->x, datapoint_->getPos()->y);
        setZValue(1);
    }

    TransferFunctionDataPoint* TransferFunctionEditorControlPoint::getPoint(){
        return datapoint_;
    }

    void TransferFunctionEditorControlPoint::setId(int id){
        id_ = id;
    }

    const int TransferFunctionEditorControlPoint::getId(){
        return id_;
    }

    TransferFunctionEditorControlPoint::TransferFunctionEditorControlPoint(){};

    TransferFunctionEditorControlPoint::~TransferFunctionEditorControlPoint(){};

    void TransferFunctionEditorControlPoint::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget* widget) {
        IVW_UNUSED_PARAM(options);
        IVW_UNUSED_PARAM(widget);
        setFlag(QGraphicsItem::ItemIsMovable, true);
        painter->setRenderHint(QPainter::Antialiasing, true);
        QPen* pen;
        float size = 10.0f;

        const vec4* col = datapoint_->getRgba();
        if (this->isSelected()){
            pen = new QPen(Qt::red, 2.5, Qt::SolidLine, Qt::RoundCap);
        } 
        else{
            pen = new QPen(Qt::black, 2.5, Qt::SolidLine, Qt::RoundCap);
        }
        painter->setPen(*pen);
        painter->drawEllipse(-size/2, -size/2, size, size);

        pen = new QPen(QColor::fromRgbF(col->r, col->g, col->b), 5.0, Qt::SolidLine, Qt::RoundCap);
        painter->setPen(*pen);
        painter->drawEllipse(-size/2,-size/2, size, size);
    }

    QRectF TransferFunctionEditorControlPoint::boundingRect() const {
        float size = 12.0f;
        float left = pos().x() - size/2;
        float top = pos().y() - size/2;
        return QRectF(-size/2, -size/2, size, size);
    }

    void TransferFunctionEditorControlPoint::mousePressEvent ( QGraphicsSceneMouseEvent *e ){
        this->setSelected(true);
    }

    void TransferFunctionEditorControlPoint::mouseReleaseEvent( QGraphicsSceneMouseEvent *e ){}

    void TransferFunctionEditorControlPoint::mouseMoveEvent(QGraphicsSceneMouseEvent * e)
    {
        QPointF* pos = new QPointF(e->scenePos());
        if (pos->x() <= 0.0f){
            pos->setX(0);
        } 
        if (pos->x() >= 255.0f){
            pos->setX(255.0f);
        }
        if (pos->y() <= 0.0f){
            pos->setY(0.0f);
        } 
        if (pos->y() >= 100.0f){
            pos->setY(100.0f);
        }

        this->setPos(*pos);
        this->datapoint_->setPos(new vec2(pos->x(), pos->y()));
        this->datapoint_->setA(pos->y()/100.0f);
    }
} // namespace