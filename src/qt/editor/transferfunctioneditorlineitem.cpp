#define _USE_MATH_DEFINES

#include <QPainter>
#include <math.h>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <inviwo/qt/editor/transferfunctioneditorlineitem.h>
#include <inviwo/qt/editor/transferfunctioneditor.h>

namespace inviwo {
	const int LEFT = 1;
	const int RIGHT = 2;

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
	float viewScale = static_cast<TransferFunctionEditor*>(this->scene())->getView()->transform().m11();
	float viewWidth = static_cast<TransferFunctionEditor*>(this->scene())->getView()->width();
	float viewHeight = static_cast<TransferFunctionEditor*>(this->scene())->getView()->height();
	QPointF start = start_->pos();
	QPointF finish = finish_->pos();
	//QPointF start = QPointF(start_->getPoint()->getPos()->x * viewWidth, start_->getPoint()->getPos()->y * viewWidth);
	//QPointF finish = QPointF(finish_->getPoint()->getPos()->x * viewHeight, finish_->getPoint()->getPos()->y * viewHeight);

	if (direction_ == LEFT){
		finish = start;
		finish.setX(start.x() - viewWidth);
	}

	if (direction_ == RIGHT){
		finish = start;
		finish.setX(start.x() + viewWidth);
	}
	viewScale = (viewScale > 10.0) ? 10.0 : viewScale;
	QPen pen = QPen();
	pen.setStyle(Qt::SolidLine);
	pen.setCapStyle(Qt::RoundCap);
	pen.setWidth(4.0/viewScale);
	isSelected() ? pen.setColor(Qt::red) : pen.setColor(Qt::black);
	painter->setPen(pen);
	painter->drawLine(start, finish);
	pen.setWidth(2.0/viewScale);
	pen.setColor(Qt::cyan);
	painter->setPen(pen);
	painter->drawLine(start, finish);
	painter->drawPath(shape());
}

QPainterPath TransferFunctionEditorLineItem::shape() const { 
	QPolygonF boundingPolygon;
	QPainterPath boundingPath;
	float viewScale = static_cast<TransferFunctionEditor*>(this->scene())->getView()->transform().m11();
	float boxWidth = 20.0f / viewScale;

	QPointF startPos = start_->pos();
	QPointF finishPos = finish_->pos();
	QPointF deltaPos = finishPos - startPos;
	float angle = atan2(deltaPos.y(), deltaPos.x());
	QPointF rotatedSize = QPointF(sin(angle), -cos(angle)) * boxWidth/2;

	boundingPolygon	<< startPos - rotatedSize	//Lower left
		<< startPos + rotatedSize	//Upper left
		<< finishPos + rotatedSize	//Upper right
		<< finishPos - rotatedSize;	//Lower right
	
	boundingPath.addPolygon(boundingPolygon);
	boundingPath.closeSubpath();
	return boundingPath; 
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
	float viewWidth = static_cast<TransferFunctionEditor*>(this->scene())->getView()->width();
	float viewHeight = static_cast<TransferFunctionEditor*>(this->scene())->getView()->height();

	QPointF delta = e->pos() - mouseDownPos_;
	QPointF startPos = start_->pos() + delta;
	QPointF finishPos = finish_->pos() + delta;
	QPointF startToFinish = finishPos - startPos;

	float minX = (start_->getLeftNeighbour()) ? start_->getLeftNeighbour()->x() + 0.001f : 0.0f;
	float maxX = (finish_->getRightNeighbour()) ? finish_->getRightNeighbour()->x() - 0.001f : viewWidth;
	float minY = 0.0;
	float maxY = viewHeight;

	if (startPos.x() < minX){
		startPos.setX(minX);
		finishPos = startPos + startToFinish;
	}
	if (finishPos.x() > maxX){
		finishPos.setX(maxX);
		startPos = finishPos - startToFinish;
	}

	if (startPos.y() < minY){
		startPos.setY(minY);
		finishPos = startPos + startToFinish;
	}
	if (startPos.y() > maxY){
		startPos.setY(maxY);
		finishPos = startPos + startToFinish;
	}

	if (finishPos.y() < minY){
		finishPos.setY(minY);
		startPos = finishPos - startToFinish;
	}
	if (finishPos.y() > maxY){
		finishPos.setY(maxY);
		startPos = finishPos - startToFinish;
	}

	//startToFinish.setX(floor(startToFinish.x() + 0.5));

	start_->setPos(startPos);
	finish_->setPos(finishPos);
	start_->getPoint()->setPos(vec2(startPos.x()/viewWidth, startPos.y()/viewHeight));
	finish_->getPoint()->setPos(vec2(finishPos.x()/viewWidth, finishPos.y()/viewHeight));
	mouseDownPos_ = e->pos();
}

TransferFunctionEditorControlPoint* TransferFunctionEditorLineItem::getStart(){return start_;}
TransferFunctionEditorControlPoint* TransferFunctionEditorLineItem::getFinish(){return finish_;}

void TransferFunctionEditorLineItem::setStart(TransferFunctionEditorControlPoint* start){start_ = start;}
void TransferFunctionEditorLineItem::setFinish(TransferFunctionEditorControlPoint* finish){finish_ = finish;}
void TransferFunctionEditorLineItem::setDirection(int dir){direction_ = dir;}
} // namespace