#include <inviwo/qt/editor/transferfunctioneditor.h>

namespace inviwo {
TransferFunctionEditor::TransferFunctionEditor(TransferFunction* transferFunction, QGraphicsView* view)
	:transferFunction_(transferFunction),
	view_(view)
{
	leftEdgeLine_ = new TransferFunctionEditorLineItem();
	rightEdgeLine_ = new TransferFunctionEditorLineItem();

	leftEdgeLine_->setDirection(1);
	rightEdgeLine_->setDirection(2);

	//VLDEnable;

	//if the editor is loaded from a saved state this adds graphicsitems to the editor for each datapoint in the Transferfunction
	for (size_t i = 0; i < transferFunction_->getNumberOfDataPoints(); i++){
		points_.push_back(new TransferFunctionEditorControlPoint(transferFunction_->getPoint(i)));
		addItem(points_.back());
		if (i > 0){
			lines_.push_back(new TransferFunctionEditorLineItem(
				points_[i],
				points_[i]));
			addItem(lines_.back());
		}
	}

	if (transferFunction_->getNumberOfDataPoints() == 0){
		addPoint(new vec2(0.0, 0.0));
		addPoint(new vec2(1.0, 1.0));
	}

	leftEdgeLine_->setStart(points_.front());
	leftEdgeLine_->setFinish(points_.front());
	rightEdgeLine_->setStart(points_.back());
	rightEdgeLine_->setFinish(points_.back());
	addItem(leftEdgeLine_);
	addItem(rightEdgeLine_);

	sortControlPoints();
	setControlPointNeighbours();
	sortLines();
	update();
}

TransferFunctionEditor::~TransferFunctionEditor(){
	for (std::vector<TransferFunctionEditorControlPoint*>::iterator p_itr = points_.begin(); p_itr != points_.end(); p_itr++){
		delete *p_itr;		
	}
	points_.clear();
}

void TransferFunctionEditor::mousePressEvent(QGraphicsSceneMouseEvent *e){
	mouseDownPos_ = e->scenePos();

	std::vector<TransferFunctionEditorControlPoint*>::iterator iter = points_.begin();

	if (e->button() == Qt::LeftButton && e->modifiers().testFlag(Qt::ControlModifier)){
		this->views().front()->setDragMode(QGraphicsView::RubberBandDrag);
	}

	if (e->button() == Qt::LeftButton && !e->modifiers().testFlag(Qt::ControlModifier)){
		if (items(e->scenePos()).isEmpty()){
			clearSelection();
			addPoint(e);
			QGraphicsScene::mousePressEvent(e);
		}
		else{
			clearSelection();
			QGraphicsScene::mousePressEvent(e);
		}
	}
	else if (e->button() == Qt::RightButton){
		if (items(e->scenePos()).isEmpty()){
			clearSelection();
		}
		else if (items(e->scenePos()).first()->type() == TransferFunctionEditorControlPoint::Type){
			removePoint((TransferFunctionEditorControlPoint*)items(e->scenePos()).first());
		}
	}
	update();
}

void TransferFunctionEditor::mouseDoubleClickEvent( QGraphicsSceneMouseEvent *e ){
	emit doubleClick();
}

void TransferFunctionEditor::mouseMoveEvent(QGraphicsSceneMouseEvent *e){
	QGraphicsScene::mouseMoveEvent(e);
	transferFunction_->sortDataPoints();
	transferFunction_->calcTransferValues();
	notifyObservers();
	sortLines();
	update();
}

void TransferFunctionEditor::mouseReleaseEvent(QGraphicsSceneMouseEvent *e){
	QGraphicsScene::mouseReleaseEvent(e);
}

void TransferFunctionEditor::keyPressEvent( QKeyEvent *e ){
	if (e->key() == Qt::Key_Delete && points_.size() > 0){
		std::vector<TransferFunctionEditorControlPoint*>::iterator iter = points_.begin();
		while (iter != points_.end()){
			if ((*iter)->isSelected()){
				iter = removePoint(*iter);
			} 
			else{
				++iter;
			}
		}
	}
}

void TransferFunctionEditor::addPoint(vec2* pos){
	pos->x = (pos->x < 0.0) ? 0.0 : pos->x;
	pos->y = (pos->y < 0.0) ? 0.0 : pos->y;
	pos->x = (pos->x > view_->width()) ? view_->width() : pos->x;
	pos->y = (pos->y > view_->height()) ? view_->height() : pos->y;

	vec4* rgba = new vec4(pos->y);

	TransferFunctionDataPoint* newPoint = new TransferFunctionDataPoint(pos, rgba);
	transferFunction_->addPoint(newPoint);
	points_.push_back(new TransferFunctionEditorControlPoint(newPoint));
	addItem(points_.back());
	points_.back()->setSelected(true);
	if (transferFunction_->getNumberOfDataPoints() > 1){
		lines_.push_back(new TransferFunctionEditorLineItem(
			points_[transferFunction_->getNumberOfDataPoints()-2], 
			points_[transferFunction_->getNumberOfDataPoints()-1]));
		addItem(lines_.back());
	}
	sortControlPoints();
	sortLines();

	leftEdgeLine_->setStart(points_.front());
	leftEdgeLine_->setFinish(points_.front());
	leftEdgeLine_->setVisible(true);

	rightEdgeLine_->setStart(points_.back());
	rightEdgeLine_->setFinish(points_.back());
	rightEdgeLine_->setVisible(true);

	setControlPointNeighbours();
	notifyObservers();
	this->update();
}

void TransferFunctionEditor::addPoint(QGraphicsSceneMouseEvent *e){

	vec2* pos = new vec2(e->scenePos().x() / view_->width(), e->scenePos().y() / view_->height());
	addPoint(pos);
}

std::vector<TransferFunctionEditorControlPoint*>::iterator TransferFunctionEditor::removePoint(TransferFunctionEditorControlPoint* target){
	std::vector<TransferFunctionEditorControlPoint*>::iterator iter;
	if (!lines_.empty()){		
		lines_.back()->setVisible(false);
		delete lines_.back();
		lines_.pop_back();
	}

	transferFunction_->removePoint(target->getPoint());
	target->setVisible(false);
	delete target;

	for (iter = points_.begin() ; iter != points_.end(); iter++){
		if ((*iter) == target){
			iter = points_.erase(iter);
			break;
		}
	}

	if (points_.size() != 0){
		leftEdgeLine_->setStart(points_.front());
		leftEdgeLine_->setFinish(points_.front());
		rightEdgeLine_->setStart(points_.back());
		rightEdgeLine_->setFinish(points_.back());
	}
	else{
		leftEdgeLine_->setVisible(false);
		rightEdgeLine_->setVisible(false);
	}

	setControlPointNeighbours();
	transferFunction_->sortDataPoints();
	transferFunction_->calcTransferValues();
	sortControlPoints();
	sortLines();
	notifyObservers();
	update();
	return iter;
}

void TransferFunctionEditor::sortLines(){
	if (lines_.size() > 0){
		for (size_t i = 0; i < transferFunction_->getNumberOfDataPoints() - 1; i++){
			lines_[i]->setStart(points_[i]);
			lines_[i]->setFinish(points_[i + 1]);
		}
	}
}

void TransferFunctionEditor::setControlPointNeighbours(){

	if (points_.size() == 0){}
	else if (points_.size() == 1){
		points_.front()->setLeftNeighbour(NULL);
		points_.front()->setRightNeighbour(NULL);
	}
	else{
		std::vector<TransferFunctionEditorControlPoint*>::iterator curr = points_.begin();
		std::vector<TransferFunctionEditorControlPoint*>::iterator prev = points_.begin();

		(*curr)->setLeftNeighbour(NULL);
		(*curr)->setRightNeighbour(NULL);
		curr++;

		while (curr != points_.end()){
			(*prev)->setRightNeighbour(*curr);
			(*curr)->setLeftNeighbour(*prev);
			prev = curr;
			curr++;
		}
		points_.back()->setRightNeighbour(NULL);
	}
}

bool myPointCompare (TransferFunctionEditorControlPoint* a, TransferFunctionEditorControlPoint* b){
	return a->getPoint()->getPos()->x < b->getPoint()->getPos()->x;
}

void TransferFunctionEditor::sortControlPoints(){
	std::sort(points_.begin(), points_.end(), myPointCompare);
}

QGraphicsView* TransferFunctionEditor::getView(){
	return view_;
}

void TransferFunctionEditor::repositionPoints(){
	for (size_t i = 0; i < points_.size(); ++i){
		points_[i]->setPos(points_[i]->getPoint()->getPos()->x * getView()->width(),points_[i]->getPoint()->getPos()->y * getView()->height());
	}
	this->update();
	this->invalidate();
}
};