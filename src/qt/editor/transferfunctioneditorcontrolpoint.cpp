#include <inviwo/qt/editor/transferfunctioneditorcontrolpoint.h>
#include <inviwo/qt/editor/transferfunctioneditor.h>

namespace inviwo {

TransferFunctionEditorControlPoint::TransferFunctionEditorControlPoint(TransferFunctionDataPoint* datapoint):datapoint_(datapoint){
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemIsSelectable);
	setFlag(QGraphicsItem::ItemSendsGeometryChanges);
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
	setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
	
	viewWidth_ = 255.0;
	viewHeight_ = 100.0;

	setPos(datapoint_->getPos()->x * viewWidth_, datapoint_->getPos()->y * viewHeight_);
	setZValue(1);
	size_ = 12.0f;
	leftNeighbour_ = NULL;
	rightNeighbour_ = NULL;
}

TransferFunctionEditorControlPoint::TransferFunctionEditorControlPoint(){};

TransferFunctionEditorControlPoint::~TransferFunctionEditorControlPoint(){};

void TransferFunctionEditorControlPoint::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget* widget) {
	IVW_UNUSED_PARAM(options);
	IVW_UNUSED_PARAM(widget);
	painter->setRenderHint(QPainter::Antialiasing, true);
	QPen pen = QPen();

	pen.setCapStyle(Qt::RoundCap);
	pen.setStyle(Qt::SolidLine);
	pen.setWidth(2.5);
	isSelected() ? pen.setColor(Qt::red) : pen.setColor(Qt::black);

	QBrush brush = QBrush(QColor::fromRgbF(datapoint_->getRgba()->r, datapoint_->getRgba()->g, datapoint_->getRgba()->b));

	painter->setPen(pen);
	painter->setBrush(brush);
	painter->drawEllipse(-size_/2.0, -size_/2.0, size_, size_);
}

QRectF TransferFunctionEditorControlPoint::boundingRect() const {
	float bboxsize = size_ + 10.0f;
	float left = pos().x() - bboxsize/2.0f;
	float top = pos().y() - bboxsize/2.0f;
	return QRectF(-bboxsize/2.0, -bboxsize/2.0f, bboxsize, bboxsize);
}

void TransferFunctionEditorControlPoint::mousePressEvent ( QGraphicsSceneMouseEvent *e ){
	datapoint_->setSelected(true);
	this->setSelected(true);
}

void TransferFunctionEditorControlPoint::mouseReleaseEvent( QGraphicsSceneMouseEvent *e ){}

void TransferFunctionEditorControlPoint::mouseMoveEvent(QGraphicsSceneMouseEvent * e){
	viewWidth_ = static_cast<TransferFunctionEditor*>(this->scene())->getView()->width();
	viewHeight_ = static_cast<TransferFunctionEditor*>(this->scene())->getView()->height();
	vec2 pos = vec2(e->scenePos().x(), e->scenePos().y());

	float minX = (getLeftNeighbour()) ? getLeftNeighbour()->x() : 0.0f;
	float maxX = (getRightNeighbour()) ? getRightNeighbour()->x() : viewWidth_;
	float minY = 0.0;
	float maxY = viewHeight_;

	pos.x = (pos.x <= minX) ? minX + 0.001f : pos.x;
	pos.x = (pos.x >= maxX) ? maxX - 0.001f : pos.x;
	pos.y = (pos.y <= minY) ? minY : pos.y;
	pos.y = (pos.y >= maxY) ? maxY : pos.y;

	this->setPos(QPointF(pos.x, pos.y));
	this->datapoint_->setPos(pos.x / viewWidth_, pos.y / viewHeight_);
	this->datapoint_->setA(pos.y/viewHeight_);
}

TransferFunctionDataPoint* TransferFunctionEditorControlPoint::getPoint() const {return datapoint_;}

TransferFunctionEditorControlPoint* TransferFunctionEditorControlPoint::getLeftNeighbour() const{return leftNeighbour_;}

TransferFunctionEditorControlPoint* TransferFunctionEditorControlPoint::getRightNeighbour() const{return rightNeighbour_;}

void TransferFunctionEditorControlPoint::setLeftNeighbour(TransferFunctionEditorControlPoint* point){leftNeighbour_ = point;}

void TransferFunctionEditorControlPoint::setRightNeighbour(TransferFunctionEditorControlPoint* point){rightNeighbour_ = point;}

QVariant TransferFunctionEditorControlPoint::itemChange(GraphicsItemChange change, const QVariant &value){
	if (change == QGraphicsItem::ItemSelectedChange){
		datapoint_->setSelected(this->isSelected());
	}

	if(change == QGraphicsItem::ItemSceneHasChanged){
		viewWidth_ = static_cast<TransferFunctionEditor*>(this->scene())->getView()->width();
		viewHeight_ = static_cast<TransferFunctionEditor*>(this->scene())->getView()->height();
		setPos(datapoint_->getPos()->x * viewWidth_, datapoint_->getPos()->y * viewHeight_);
	}
	return QGraphicsItem::itemChange(change, value);
}
} // namespace