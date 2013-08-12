#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <inviwo/qt/editor/transferfunctioneditorlineitem.h>

namespace inviwo {

	TransferFunctionEditorLineItem::TransferFunctionEditorLineItem(TransferFunctionEditorControlPoint* start, TransferFunctionEditorControlPoint* finish):
	start_(start),
	finish_(finish),
	direction_(0)
    {
		setFlag(QGraphicsItem::ItemIsMovable);
		setFlag(QGraphicsItem::ItemIsSelectable);
		setFlag(QGraphicsItem::ItemSendsGeometryChanges);
		setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
		setFlag(QGraphicsItem::ItemIgnoresTransformations, false);
        setZValue(0);
    };

	TransferFunctionEditorLineItem::TransferFunctionEditorLineItem(TransferFunctionEditorControlPoint* start, int dir):
	start_(start),
	finish_(start),
	direction_(dir)
	{
		setFlag(QGraphicsItem::ItemIsMovable);
		setFlag(QGraphicsItem::ItemIsSelectable);
		setFlag(QGraphicsItem::ItemSendsGeometryChanges);
		setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
		setFlag(QGraphicsItem::ItemIgnoresTransformations, false);
		setZValue(0);
	};

    TransferFunctionEditorLineItem::TransferFunctionEditorLineItem(){};

    TransferFunctionEditorLineItem::~TransferFunctionEditorLineItem(){};


    void TransferFunctionEditorLineItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget* widget) {
        IVW_UNUSED_PARAM(options);
        IVW_UNUSED_PARAM(widget);
        const vec2* start = start_->getPoint()->getPos();
		const vec2* finish;
		switch (direction_)
		{
			case 0:
				finish = finish_->getPoint()->getPos();
			break;
			case 1:
				finish = new vec2(start_->getPoint()->getPos()->x - 255.0, start_->getPoint()->getPos()->y);
			break;
			case 2:
				finish = new vec2(start_->getPoint()->getPos()->x + 255.0, start_->getPoint()->getPos()->y);
			break;
			default:
				finish = finish_->getPoint()->getPos();
			break;
		}
		
		QPen* pen;

        painter->setRenderHint(QPainter::Antialiasing, true);

		if (this->isSelected()){
			pen = new QPen(Qt::red, 3.0f, Qt::SolidLine, Qt::RoundCap);
		} 
		else{
			pen = new QPen(Qt::black, 3.0f, Qt::SolidLine, Qt::RoundCap);
		}

        painter->setPen(*pen);
        painter->drawLine(start->x, start->y, finish->x, finish->y);

        pen = new QPen(Qt::cyan, 1.0, Qt::SolidLine, Qt::RoundCap);
        painter->setPen(*pen);
        painter->drawLine(start->x, start->y, finish->x, finish->y);

		delete pen;
    }

    QRectF TransferFunctionEditorLineItem::boundingRect() const {
        return QRectF(start_->getPoint()->getPos()->x, start_->getPoint()->getPos()->y, finish_->getPoint()->getPos()->x - start_->getPoint()->getPos()->x, finish_->getPoint()->getPos()->y - start_->getPoint()->getPos()->y);
    }
    void TransferFunctionEditorLineItem::mousePressEvent ( QGraphicsSceneMouseEvent *e ){
		mouseDownPos_ = e->scenePos();
		this->setSelected(true);
	}

    void TransferFunctionEditorLineItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *e ){
	}

    void TransferFunctionEditorLineItem::mouseMoveEvent(QGraphicsSceneMouseEvent * e){
		vec2 deltaPos = vec2(e->pos().x()- mouseDownPos_.x() , e->pos().y() - mouseDownPos_.y()); 
		vec2 newStartPos = *start_->getPoint()->getPos() + deltaPos;
		vec2 newFinishPos = *finish_->getPoint()->getPos() + deltaPos;

		float min = (start_->getLeftNeighbour()) ? start_->getLeftNeighbour()->getPoint()->getPos()->x + 1.0 : 0.0f;
		float max = (finish_->getRightNeighbour()) ? finish_->getRightNeighbour()->getPoint()->getPos()->x - 1.0 : 255.0f;

		if (newStartPos.x < min || newFinishPos.x > max){deltaPos.x = 0.0;}
		if (newStartPos.y <  0.0 || newStartPos.y > 100.0){deltaPos.y = 0.0;}
		if (newFinishPos.y <  0.0 || newFinishPos.y > 100.0){deltaPos.y = 0.0;}

		deltaPos.x = floor(deltaPos.x + 0.5);

		newStartPos = *start_->getPoint()->getPos() + deltaPos;
		newFinishPos = *finish_->getPoint()->getPos() + deltaPos;

		start_->setPos(QPointF(newStartPos.x, newStartPos.y));
		finish_->setPos(QPointF(newFinishPos.x, newFinishPos.y));
		start_->getPoint()->setPos(newStartPos);
		finish_->getPoint()->setPos(newFinishPos);
		mouseDownPos_ = e->pos();
	}


	TransferFunctionEditorControlPoint* TransferFunctionEditorLineItem::getStart(){return start_;}
	TransferFunctionEditorControlPoint* TransferFunctionEditorLineItem::getFinish(){return finish_;}

	void TransferFunctionEditorLineItem::setStart(TransferFunctionEditorControlPoint* start){start_ = start;}
	void TransferFunctionEditorLineItem::setFinish(TransferFunctionEditorControlPoint* finish){finish_ = finish;}
	void TransferFunctionEditorLineItem::setDirection(int dir){direction_ = dir;}
} // namespace