/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Viktor Axelsson
 *
 **********************************************************************/

#include <inviwo/qt/widgets/properties/transferfunctioneditor.h>

namespace inviwo {
TransferFunctionEditor::TransferFunctionEditor(TransferFunction* transferFunction, QGraphicsView* view)
    : view_(view)
    , transferFunction_(transferFunction) {
        
    leftEdgeLine_ = new TransferFunctionEditorLineItem();
    rightEdgeLine_ = new TransferFunctionEditorLineItem();

    leftEdgeLine_->setDirection(1);
    rightEdgeLine_->setDirection(2);

    //VLDEnable;

    //if the editor is loaded from a saved state this adds graphicsitems to the editor for each datapoint in the Transferfunction
    for (int i = 0; i < static_cast<int>(transferFunction_->getNumberOfDataPoints()); i++) {
        controlPoints_.push_back(new TransferFunctionEditorControlPoint(transferFunction_->getPoint(i)));
        addItem(controlPoints_.back());
        if (i > 0) {
            lines_.push_back(new TransferFunctionEditorLineItem(
                controlPoints_[i],
                controlPoints_[i]));
            addItem(lines_.back());
        }
    }

    if (transferFunction_->getNumberOfDataPoints() == 0) {
        addPoint(new vec2(0.0, 0.0));
        addPoint(new vec2(1.0, 1.0));
    }

    leftEdgeLine_->setStart(controlPoints_.front());
    leftEdgeLine_->setFinish(controlPoints_.front());
    rightEdgeLine_->setStart(controlPoints_.back());
    rightEdgeLine_->setFinish(controlPoints_.back());
    addItem(leftEdgeLine_);
    addItem(rightEdgeLine_);

    sortControlPoints();
    setControlPointNeighbours();
    sortLines();
    update();
}

TransferFunctionEditor::~TransferFunctionEditor() {
    for (std::vector<TransferFunctionEditorControlPoint*>::iterator p_itr = controlPoints_.begin(); p_itr != controlPoints_.end(); p_itr++) {
        delete *p_itr;		
    }
    controlPoints_.clear();
}

void TransferFunctionEditor::mousePressEvent(QGraphicsSceneMouseEvent *e) {
    mouseDownPos_ = e->scenePos();

    if (e->button() == Qt::LeftButton && e->modifiers().testFlag(Qt::ControlModifier)) {
        this->views().front()->setDragMode(QGraphicsView::RubberBandDrag);
    }

    if (e->button() == Qt::LeftButton && !e->modifiers().testFlag(Qt::ControlModifier)) {
        if (items(e->scenePos()).isEmpty()) {
            clearSelection();
            addPoint(e);
            QGraphicsScene::mousePressEvent(e);
        }
        else{
            clearSelection();
            QGraphicsScene::mousePressEvent(e);
        }
    }
    else if (e->button() == Qt::RightButton) {
        if (items(e->scenePos()).isEmpty()) {
            clearSelection();
        }
        else if (items(e->scenePos()).first()->type() == TransferFunctionEditorControlPoint::Type) {
            removePoint((TransferFunctionEditorControlPoint*)items(e->scenePos()).first());
        }
    }
    update();
}

void TransferFunctionEditor::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e) {
    emit doubleClick();
}

void TransferFunctionEditor::mouseMoveEvent(QGraphicsSceneMouseEvent *e) {
    QGraphicsScene::mouseMoveEvent(e);
    transferFunction_->sortDataPoints();
    transferFunction_->calcTransferValues();
    sortLines();
    notifyObservers();
}

void TransferFunctionEditor::mouseReleaseEvent(QGraphicsSceneMouseEvent *e) {
    QGraphicsScene::mouseReleaseEvent(e);
}

void TransferFunctionEditor::keyPressEvent(QKeyEvent *e) {
    if (e->key() == 'A' && e->modifiers().testFlag(Qt::ControlModifier)) {
        QList<QGraphicsItem *> itemList = this->items();
        foreach(QGraphicsItem *item, itemList) {
            item->setSelected(true);
        }
    }

    if (e->key() == 'G' && e->modifiers().testFlag(Qt::ControlModifier)) {
        //TransferFunctionEditorControlPointGroup* temp = new TransferFunctionEditorControlPointGroup();
        QList<QGraphicsItem *> itemList = this->items();
        foreach(QGraphicsItem *item, itemList) {
            //temp.addPoint(static_cast<TransferFunctionEditorControlPoint*<item);
        }
        //controlPointGroups_.push_back(temp);
    }

    if (e->key() == Qt::Key_Delete && controlPoints_.size() > 0) {
        std::vector<TransferFunctionEditorControlPoint*>::iterator iter = controlPoints_.begin();
        while (iter != controlPoints_.end()) {
            if ((*iter)->isSelected()) {
                iter = removePoint(*iter);
            } 
            else{
                ++iter;
            }
        }
    }
}

void TransferFunctionEditor::addPoint(QGraphicsSceneMouseEvent *e) {
    float width = zoomRangeXMax_ - zoomRangeXMin_;
    float height = zoomRangeYMax_ - zoomRangeYMin_;
    float newX = e->scenePos().x() / view_->width();
    float newY = e->scenePos().y() / view_->height();
    newX = zoomRangeXMin_ + width * newX;
    newY = zoomRangeYMin_ + height * newY;
    vec2* pos = new vec2(newX, newY);   
    addPoint(pos);
}

void TransferFunctionEditor::addPoint(vec2* pos) {
    pos->x = (pos->x < 0.0) ? 0.0 : pos->x;
    pos->y = (pos->y < 0.0) ? 0.0 : pos->y;
    pos->x = (pos->x > view_->width()) ? view_->width() : pos->x;
    pos->y = (pos->y > view_->height()) ? view_->height() : pos->y;

    vec4* rgba = new vec4(pos->y);

    TransferFunctionDataPoint* newPoint = new TransferFunctionDataPoint(pos, rgba);
    transferFunction_->addPoint(newPoint);
    controlPoints_.push_back(new TransferFunctionEditorControlPoint(newPoint));
    addItem(controlPoints_.back());
    controlPoints_.back()->setSelected(true);
    if (transferFunction_->getNumberOfDataPoints() > 1) {
        lines_.push_back(new TransferFunctionEditorLineItem(
            controlPoints_[transferFunction_->getNumberOfDataPoints()-2], 
            controlPoints_[transferFunction_->getNumberOfDataPoints()-1]));
        addItem(lines_.back());
    }
    sortControlPoints();
    setControlPointNeighbours();
    sortLines();

    leftEdgeLine_->setStart(controlPoints_.front());
    leftEdgeLine_->setFinish(controlPoints_.front());
    leftEdgeLine_->setVisible(true);

    rightEdgeLine_->setStart(controlPoints_.back());
    rightEdgeLine_->setFinish(controlPoints_.back());
    rightEdgeLine_->setVisible(true);

    notifyObservers();
    repositionPoints();
    update();
}

std::vector<TransferFunctionEditorControlPoint*>::iterator TransferFunctionEditor::removePoint(TransferFunctionEditorControlPoint* target) {
    std::vector<TransferFunctionEditorControlPoint*>::iterator iter;
    if (!lines_.empty()) {		
        lines_.back()->setVisible(false);
        delete lines_.back();
        lines_.pop_back();
    }

    transferFunction_->removePoint(target->getPoint());
    target->setVisible(false);
    delete target;

    for (iter = controlPoints_.begin() ; iter != controlPoints_.end(); iter++) {
        if ((*iter) == target) {
            iter = controlPoints_.erase(iter);
            break;
        }
    }

    if (controlPoints_.size() != 0) {
        leftEdgeLine_->setStart(controlPoints_.front());
        leftEdgeLine_->setFinish(controlPoints_.front());
        rightEdgeLine_->setStart(controlPoints_.back());
        rightEdgeLine_->setFinish(controlPoints_.back());
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

void TransferFunctionEditor::sortLines() {
    if (lines_.size() > 0) {
        for (size_t i = 0; i < transferFunction_->getNumberOfDataPoints() - 1; i++) {
            lines_[i]->setStart(controlPoints_[i]);
            lines_[i]->setFinish(controlPoints_[i + 1]);
        }
    }
}

void TransferFunctionEditor::setControlPointNeighbours() {
    if (controlPoints_.size() == 0) {}
    else if (controlPoints_.size() == 1) {
        controlPoints_.front()->setLeftNeighbour(NULL);
        controlPoints_.front()->setRightNeighbour(NULL);
    }
    else{
        std::vector<TransferFunctionEditorControlPoint*>::iterator curr = controlPoints_.begin();
        std::vector<TransferFunctionEditorControlPoint*>::iterator prev = controlPoints_.begin();

        (*curr)->setLeftNeighbour(NULL);
        (*curr)->setRightNeighbour(NULL);
        curr++;

        while(curr != controlPoints_.end()) {
            (*prev)->setRightNeighbour(*curr);
            (*curr)->setLeftNeighbour(*prev);
            prev = curr;
            curr++;
        }
        controlPoints_.back()->setRightNeighbour(NULL);
    }
}

bool myPointCompare(TransferFunctionEditorControlPoint* a, TransferFunctionEditorControlPoint* b) {
    return a->getPoint()->getPos()->x < b->getPoint()->getPos()->x;
}

void TransferFunctionEditor::sortControlPoints() {
    std::sort(controlPoints_.begin(), controlPoints_.end(), myPointCompare);
}

QGraphicsView* TransferFunctionEditor::getView() {
    return view_;
}

void TransferFunctionEditor::repositionPoints() {
    float viewWidth_ = getView()->width();
    float viewHeight_ = getView()->height();

    for (size_t i = 0; i < controlPoints_.size(); ++i) {
        float zoomRangeMinX = getZoomRangeXMin();
        float zoomRangeMaxX = getZoomRangeXMax();

        float zoomRangeMinY = getZoomRangeYMin();
        float zoomRangeMaxY = getZoomRangeYMax();

        float datapointXpos = controlPoints_[i]->getPoint()->getPos()->x;
        float datapointYpos = controlPoints_[i]->getPoint()->getPos()->y;

        float newX = (datapointXpos - zoomRangeMinX)  * viewWidth_ / (zoomRangeMaxX - zoomRangeMinX);
        float newY = (datapointYpos - zoomRangeMinY)  * viewHeight_ / (zoomRangeMaxY - zoomRangeMinY);
        controlPoints_[i]->setPos(newX, newY);
    }
    this->update();
    this->invalidate();
}

const float TransferFunctionEditor::getZoomRangeXMin() {
    return zoomRangeXMin_;
}

void TransferFunctionEditor::setZoomRangeXMin(float min) {
    zoomRangeXMin_ = min;
}

const float TransferFunctionEditor::getZoomRangeXMax() {
    return zoomRangeXMax_;
}

void TransferFunctionEditor::setZoomRangeXMax(float max) {
    zoomRangeXMax_ = max;
}

const float TransferFunctionEditor::getZoomRangeYMin() {
    return zoomRangeYMin_;
}

void TransferFunctionEditor::setZoomRangeYMin(float min) {
    zoomRangeYMin_ = min;
}

const float TransferFunctionEditor::getZoomRangeYMax() {
    return zoomRangeYMax_;
}

void TransferFunctionEditor::setZoomRangeYMax(float max) {
    zoomRangeYMax_ = max;
}

};