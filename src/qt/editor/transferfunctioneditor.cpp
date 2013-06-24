#include <inviwo/qt/editor/transferfunctioneditor.h>

namespace inviwo {
	TransferFunctionEditor::TransferFunctionEditor(TransferFunction* transferFunc)
		:transferFunction_(transferFunc)
	{
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
		sortControlPoints();
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

		if (points_.size() > 0){}
		std::vector<TransferFunctionEditorControlPoint*>::iterator iter = points_.begin();

		if (e->button() == Qt::LeftButton){

			for (std::vector<TransferFunctionEditorLineItem*>::iterator lineiter = lines_.begin(); lineiter != lines_.end(); lineiter++){
				(*lineiter)->setSelected(false);
			}

			if(!e->modifiers().testFlag(Qt::ControlModifier)){
				for (iter = points_.begin(); iter != points_.end(); iter++){
					(*iter)->getPoint()->setSelected(false);
					(*iter)->setSelected(false);
				}
			}
				if (itemAt(e->scenePos()) != NULL){
				QGraphicsScene::mousePressEvent(e); // this forwards the event to the item
			}
		}
		else if (e->button() == Qt::RightButton){
			if (itemAt(e->scenePos()) == NULL){}
			else if (itemAt(e->scenePos())->type() == TransferFunctionEditorControlPoint::Type){
				removePoint((TransferFunctionEditorControlPoint*)itemAt(e->scenePos()));
			}
			for (iter = points_.begin(); iter != points_.end(); iter++){
				(*iter)->getPoint()->setSelected(false);
			}
		}
		update();
	}

	void TransferFunctionEditor::mouseMoveEvent(QGraphicsSceneMouseEvent *e){
		QGraphicsScene::mouseMoveEvent(e);
		transferFunction_->sortDataPoints();
		transferFunction_->calcTransferValues();
		//(parent_)->updateFromProperty();
		notifyObservers();
		sortLines();
		update();
	}

	void TransferFunctionEditor::mouseReleaseEvent(QGraphicsSceneMouseEvent *e){
		float dist = sqrt( pow(e->scenePos().x() - mouseDownPos_.x(), 2) + pow(e->scenePos().y() - mouseDownPos_.y(), 2));

		if (e->button() == Qt::LeftButton && dist < 10.0f){
			if (itemAt(mouseDownPos_) == NULL){
				//if (itemAt(mouseDownPos_) == NULL || itemAt(mouseDownPos_)->type() == TransferFunctionEditorLineItem::Type){
				addPoint(e);
			}
		}
		QGraphicsScene::mouseReleaseEvent(e);
	}


	void TransferFunctionEditor::keyPressEvent( QKeyEvent *e ){
		if (e->key() == Qt::Key_Delete && points_.size() > 0){
			std::vector<TransferFunctionEditorControlPoint*>::iterator iter = points_.begin();
			while (iter != points_.end()){
				if ((*iter)->getPoint()->isSelected()){
					iter = removePoint(*iter);
				} 
				else{
					++iter;
				}
			}
		}
	}

	void TransferFunctionEditor::addPoint(QGraphicsSceneMouseEvent *e){
		vec2* pos = new vec2(e->scenePos().x(), e->scenePos().y());
		pos->x = floor(pos->x + 0.5f);
		vec4* rgba = new vec4(e->scenePos().y()/100.0f);
		TransferFunctionDataPoint* newPoint = new TransferFunctionDataPoint(pos, rgba);
		transferFunction_->addPoint(newPoint);
		points_.push_back(new TransferFunctionEditorControlPoint(newPoint));
		addItem(points_.back());
		if (transferFunction_->getNumberOfDataPoints() > 1){
			lines_.push_back(new TransferFunctionEditorLineItem(
				points_[transferFunction_->getNumberOfDataPoints()-2], 
				points_[transferFunction_->getNumberOfDataPoints()-1]));
			addItem(lines_.back());
		}

		points_.back()->getPoint()->setSelected(true);
		points_.back()->setSelected(true);
		sortControlPoints();
		sortLines();
		setControlPointNeighbours();
		//parent_->updateFromProperty();
		notifyObservers();
		this->update();
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

		setControlPointNeighbours();
		transferFunction_->sortDataPoints();
		transferFunction_->calcTransferValues();
		sortControlPoints();
		sortLines();
		//parent_->updateFromProperty();
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
};