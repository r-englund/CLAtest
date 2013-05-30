#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <inviwo/qt/editor/transferfunctioneditorcontrolpoint.h>

namespace inviwo {

    TransferFunctionEditorControlPoint::TransferFunctionEditorControlPoint(TransferFunctionDataPoint* datapoint):datapoint_(datapoint){
		setFlag(QGraphicsItem::ItemIsMovable);
		setFlag(QGraphicsItem::ItemIsSelectable);
        setFlag(QGraphicsItem::ItemSendsGeometryChanges);
        setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
		setFlag(QGraphicsItem::ItemIgnoresTransformations, true);

        setPos(datapoint_->getPos()->x, datapoint_->getPos()->y);
        setZValue(1);
		size_ = 12.0f;
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
        painter->setRenderHint(QPainter::Antialiasing, true);
        QPen* pen;
		QBrush* brush;

        const vec4* col = datapoint_->getRgba();
		brush = new QBrush(QColor::fromRgbF(col->r, col->g, col->b));
		
        if (this->isSelected()){
            pen = new QPen(Qt::red, 2.5, Qt::SolidLine, Qt::RoundCap);
        } 
        else{
            pen = new QPen(Qt::black, 2.5, Qt::SolidLine, Qt::RoundCap);
        }
        painter->setPen(*pen);
		painter->setBrush(*brush);
		painter->drawEllipse(-size_/2, -size_/2, size_, size_);
    }

    QRectF TransferFunctionEditorControlPoint::boundingRect() const {
		float bboxsize = size_ + 10.0f;
        float left = pos().x() - bboxsize/2;
        float top = pos().y() - bboxsize/2;
        return QRectF(-bboxsize/2, -bboxsize/2, bboxsize, bboxsize);
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
		
		pos->setX(floor(pos->x() + 0.5));

        this->setPos(*pos);
        this->datapoint_->setPos(new vec2(pos->x(), pos->y()));
        this->datapoint_->setA(pos->y()/100.0f);
    }
} // namespace