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
    : QGraphicsScene()
    , transferFunction_(transferFunction)
    , view_(view)
{

    // initialize editor with current tf
    for (size_t i=0; i<transferFunction_->getNumberOfDataPoints(); i++) {
        TransferFunctionDataPoint* dataPoint = transferFunction_->getPoint(i);
        vec2 pos = *dataPoint->getPos();
        pos.x *= view_->width();
        pos.y *= view_->height();
        addControlPoint(QPointF(pos.x,pos.y), dataPoint);
    }

    // if the transfer function is empty, initialize it with simple ramp
    // TODO: move to tf core (TR)
    if (transferFunction_->getNumberOfDataPoints() == 0) {
        addControlPoint(QPointF(0.0, 0.0));
        addControlPoint(QPointF(1.0, 1.0));
    }
}

TransferFunctionEditor::~TransferFunctionEditor() {
    for (size_t i=0; i<controlPoints_.size(); i++)
        delete controlPoints_[i];
    controlPoints_.clear();

    for (size_t i=0; i<lines_.size(); i++)
        delete lines_[i];
    lines_.clear();
}

void TransferFunctionEditor::mousePressEvent(QGraphicsSceneMouseEvent* e) {
    TransferFunctionEditorControlPoint* controlPointGraphicsItem = getControlPointGraphicsItemAt(e->scenePos());
    if (e->button() == Qt::LeftButton) {
        if (!controlPointGraphicsItem) {
            if (e->modifiers()==Qt::NoModifier) {
                addControlPoint(e->scenePos());
            } else if (e->modifiers()==Qt::ControlModifier) {
                views().front()->setDragMode(QGraphicsView::RubberBandDrag);
            }
        }
    }
    
    else if (e->button() == Qt::RightButton) {
        if (controlPointGraphicsItem) {
            removeControlPoint(controlPointGraphicsItem);
        }
    }
    QGraphicsScene::mousePressEvent(e);
}

void TransferFunctionEditor::mouseMoveEvent(QGraphicsSceneMouseEvent* e) {
    QGraphicsScene::mouseMoveEvent(e);
}

void TransferFunctionEditor::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {
    QGraphicsScene::mouseReleaseEvent(e);
}

void TransferFunctionEditor::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* e) {
    emit doubleClick();
    QGraphicsScene::mouseDoubleClickEvent(e);
}

void TransferFunctionEditor::keyPressEvent(QKeyEvent* e) {
    // mark all keys
    if (e->key() == 'A' && e->modifiers()==Qt::ControlModifier) {
        QList<QGraphicsItem *> itemList = items();
        foreach(QGraphicsItem *item, itemList) {
            item->setSelected(true);
        }
    }

    /*
    // group selected keys
    if (e->key() == 'G' && e->modifiers()==Qt::ControlModifier) {
        //TransferFunctionEditorControlPointGroup* temp = new TransferFunctionEditorControlPointGroup();
        QList<QGraphicsItem *> itemList = this->items();
        foreach(QGraphicsItem *item, itemList) {
            //temp.addPoint(static_cast<TransferFunctionEditorControlPoint*<item);
        }
        //controlPointGroups_.push_back(temp);
    }
    */

    // delete selected keys
    if (e->key() == Qt::Key_Delete) {
        for (size_t i=0; i<controlPoints_.size(); i++)
            if (controlPoints_[i]->isSelected())
                removeControlPoint(controlPoints_[i]);
    }
}

/*
void TransferFunctionEditor::addControlPoint(QGraphicsSceneMouseEvent *e) {
    float width = zoomRangeXMax_ - zoomRangeXMin_;
    float height = zoomRangeYMax_ - zoomRangeYMin_;
    float newX = e->scenePos().x() / view_->width();
    float newY = e->scenePos().y() / view_->height();
    newX = zoomRangeXMin_ + width * newX;
    newY = zoomRangeYMin_ + height * newY;
    vec2* pos = new vec2(newX, newY);   
    addControlPoint(pos);
}
*/

void TransferFunctionEditor::addControlPoint(QPointF pos) {
    ivwAssert(pos.x()>=0.0 && pos.x()<view_->width(), "Transfer function control point's x coordinate out of range.");
    ivwAssert(pos.y()>=0.0 && pos.y()<view_->height(), "Transfer function control point's y coordinate out of range.");

    // suggest a color
    vec4 rgba = vec4(0.5f);

    // add control point to editor and transfer function
    TransferFunctionDataPoint* dataPoint = new TransferFunctionDataPoint(vec2(pos.x()/view_->width(),pos.y()/view_->height()), rgba);
    transferFunction_->addPoint(dataPoint);
    addControlPoint(pos, dataPoint);
}

void TransferFunctionEditor::addControlPoint(QPointF pos, TransferFunctionDataPoint* dataPoint) {
    TransferFunctionEditorControlPoint* pointGraphicsItem = new TransferFunctionEditorControlPoint(dataPoint);
    pointGraphicsItem->setPos(pos);
    controlPoints_.push_back(pointGraphicsItem);
    addItem(pointGraphicsItem);
    updateControlPointView();
}

void TransferFunctionEditor::removeControlPoint(TransferFunctionEditorControlPoint* controlPoint) {
    transferFunction_->removePoint(controlPoint->getPoint());
    removeItem(controlPoint);
    controlPoints_.erase(std::remove(controlPoints_.begin(), controlPoints_.end(), controlPoint), controlPoints_.end());
    delete controlPoint;
    updateControlPointView();
}

bool controlPointComparison(TransferFunctionEditorControlPoint* controlPoint0, TransferFunctionEditorControlPoint* controlPoint1) {
    return (controlPoint0->x() < controlPoint1->x());
}

void TransferFunctionEditor::updateLines() {
    // cleanup old lines
    for (size_t i=0; i<lines_.size(); i++) {
        removeItem(lines_[i]);
        delete lines_[i];
    }
    lines_.clear();

    // sort control points
    std::sort(controlPoints_.begin(), controlPoints_.end(), controlPointComparison);

    // add new lines
    TransferFunctionEditorLineItem* firstLine = new TransferFunctionEditorLineItem(QPointF(0.0,controlPoints_[0]->pos().y()), controlPoints_[0]->pos());
    lines_.push_back(firstLine);
    addItem(firstLine);
    for (size_t i=0; i<controlPoints_.size()-1; i++) {
        TransferFunctionEditorLineItem* lineGraphicsItem = new TransferFunctionEditorLineItem(controlPoints_[i]->pos(), controlPoints_[i+1]->pos());
        lines_.push_back(lineGraphicsItem);
        addItem(lineGraphicsItem);
    }
    TransferFunctionEditorLineItem* lastLine = new TransferFunctionEditorLineItem(controlPoints_[controlPoints_.size()-1]->pos(),
                                                                                  QPointF(view_->width(), controlPoints_[controlPoints_.size()-1]->pos().y()));
    lines_.push_back(lastLine);
    addItem(lastLine);
}

void TransferFunctionEditor::updateControlPointView() {
    // update lines and repaint
    updateLines();
    update();
    // initiate transfer function update
    emit controlPointsChanged();
}



/*
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
    update();
    invalidate();
}
*/

TransferFunctionEditorControlPoint* TransferFunctionEditor::getControlPointGraphicsItemAt(const QPointF pos) const {
    QList<QGraphicsItem*> graphicsItems = items(pos);
    for (int i=0; i<graphicsItems.size(); i++) {
        TransferFunctionEditorControlPoint* controlPointGraphicsItem = qgraphicsitem_cast<TransferFunctionEditorControlPoint*>(graphicsItems[i]);
        if (controlPointGraphicsItem)
            return controlPointGraphicsItem;
    }
    return 0;
}

};