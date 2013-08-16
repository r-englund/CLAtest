#include <QPainter>
#include <math.h>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <inviwo/qt/editor/transferfunctioneditorlineitem.h>

namespace inviwo {

TransferFunctionEditorLineItem::TransferFunctionEditorLineItem(){
	initiateLineItem();
};

TransferFunctionEditorLineItem::TransferFunctionEditorLineItem(TransferFunctionEditorControlPoint* start, TransferFunctionEditorControlPoint* finish):
start_(start),
	finish_(finish),
	direction_(0)
{
	initiateLineItem();
};

TransferFunctionEditorLineItem::TransferFunctionEditorLineItem(TransferFunctionEditorControlPoint* start, int dir):
start_(start),
	finish_(start),
	direction_(dir)
{
	initiateLineItem();
};

void TransferFunctionEditorLineItem::initiateLineItem(){
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemIsSelectable, false);
	setFlag(QGraphicsItem::ItemSendsGeometryChanges);
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
	setFlag(QGraphicsItem::ItemIgnoresTransformations, false);
	setZValue(0);
}

TransferFunctionEditorLineItem::~TransferFunctionEditorLineItem(){};


void TransferFunctionEditorLineItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget* widget) {
	IVW_UNUSED_PARAM(options);
	IVW_UNUSED_PARAM(widget);
	painter->setRenderHint(QPainter::Antialiasing, true);

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
	QPen* pen = new QPen();
	pen->setStyle(Qt::SolidLine);
	pen->setCapStyle(Qt::RoundCap);
	pen->setWidth(6.0);
	this->isSelected() ? pen->setColor(Qt::red) : pen->setColor(Qt::black);
	
	painter->setPen(*pen);
	painter->drawLine(start->x, start->y, finish->x, finish->y);

	pen->setWidth(2.0);
	pen->setColor(Qt::cyan);

	painter->setPen(*pen);
	painter->drawLine(start->x, start->y, finish->x, finish->y);

	delete pen;
}

QPainterPath TransferFunctionEditorLineItem::shape() const { 
	QPainterPath path;
	QPolygonF poly;

	vec2 sPos = vec2(start_->getPoint()->getPos()->x, start_->getPoint()->getPos()->y);
	vec2 fPos = vec2(finish_->getPoint()->getPos()->x, finish_->getPoint()->getPos()->y);

	float size = 10.0f;

	poly<< QPointF(sPos.x - size, sPos.y + size)
		<< QPointF(sPos.x - size, sPos.y - size)
		<< QPointF(fPos.x + size, fPos.y - size)
		<< QPointF(fPos.x + size, fPos.y + size);

	path.addPolygon(poly);

	path.closeSubpath();
	return path; 
} 

QRectF TransferFunctionEditorLineItem::boundingRect() const {
	return shape().boundingRect();
}

void TransferFunctionEditorLineItem::mousePressEvent ( QGraphicsSceneMouseEvent *e ){
	mouseDownPos_ = e->scenePos();
	this->setSelected(true);
}

void TransferFunctionEditorLineItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *e ){}

void TransferFunctionEditorLineItem::mouseMoveEvent(QGraphicsSceneMouseEvent * e){
	vec2 deltaPos = vec2(e->pos().x()- mouseDownPos_.x() , e->pos().y() - mouseDownPos_.y()); 
	vec2 newStartPos = *start_->getPoint()->getPos() + deltaPos;
	vec2 newFinishPos = *finish_->getPoint()->getPos() + deltaPos;
	vec2 lineVector = *finish_->getPoint()->getPos() - *start_->getPoint()->getPos();

	float minX = (start_->getLeftNeighbour()) ? start_->getLeftNeighbour()->getPoint()->getPos()->x + 1.0 : 0.0f;
	float maxX = (finish_->getRightNeighbour()) ? finish_->getRightNeighbour()->getPoint()->getPos()->x - 1.0 : 255.0f;
	float minY = 0.0;
	float maxY = 100.0;

	if (newStartPos.x < minX){
		newStartPos.x = minX;
		newFinishPos = newStartPos + lineVector;
	}
	if (newFinishPos.x > maxX){
		newFinishPos.x = maxX;
		newStartPos = newFinishPos - lineVector;
	}

	if (newStartPos.y < minY){
		newStartPos.y = minY;
		newFinishPos = newStartPos + lineVector;
	}
	if (newStartPos.y > maxY){
		newStartPos.y = maxY;
		newFinishPos = newStartPos + lineVector;
	}

	if (newFinishPos.y < minY){
		newFinishPos.y = minY;
		newStartPos = newFinishPos - lineVector;
	}
	if (newFinishPos.y > maxY){
		newFinishPos.y = maxY;
		newStartPos = newFinishPos - lineVector;
	}

	deltaPos.x = floor(deltaPos.x + 0.5);

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