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
    , view_(view)
    , transferFunction_(transferFunction) {
        
    setSceneRect(0.0, 0.0, 512.0, 256.0);
    // initialize editor with current tf
    for (size_t i=0; i<transferFunction_->getNumDataPoints(); i++) {
        TransferFunctionDataPoint* dataPoint = transferFunction_->getPoint(static_cast<int>(i));
        vec2 pos = *dataPoint->getPos();
        pos.x *= width();
        pos.y *= height();
        addControlPoint(QPointF(pos.x,pos.y), dataPoint);
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

void TransferFunctionEditor::resetTransferFunction() {
    for (size_t i=0; i<controlPoints_.size(); i++) {
        transferFunction_->removePoint(controlPoints_[i]->getPoint());
        removeItem(controlPoints_[i]);
        delete controlPoints_[i];
    }
    controlPoints_.clear();
    addControlPoint(QPointF(0.0*(width()-1), 0.0*(height()-1)), vec4(0.0f));
    addControlPoint(QPointF(1.0*(width()-1), 1.0*(height()-1)), vec4(1.0f));
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
    // delete selected keys
    if (e->key() == Qt::Key_Delete) {
        for (size_t i=0; i<controlPoints_.size(); i++)
            if (controlPoints_[i]->isSelected())
                removeControlPoint(controlPoints_[i]);
    }
}

void TransferFunctionEditor::addControlPoint(QPointF pos) {
    // determine color
    vec4 color = vec4(0.5f, 0.5f, 0.5f, pos.y()/height());
    if (controlPoints_.size() > 0) {
        int leftNeighborID = 0;
        int rightNeighborID = 0;
        for (size_t i=0; i<controlPoints_.size(); i++)
            if (controlPoints_[i]->pos().x()<=pos.x()) leftNeighborID=static_cast<int>(i);
            else if (rightNeighborID==0 && controlPoints_[i]->pos().x()>pos.x()) rightNeighborID=static_cast<int>(i);
        const vec4* colorL = controlPoints_[leftNeighborID]->getPoint()->getRgba();
        const vec4* colorR = controlPoints_[rightNeighborID]->getPoint()->getRgba();
        float a = 0.5f;
        float denom = controlPoints_[rightNeighborID]->pos().x() - controlPoints_[leftNeighborID]->pos().x();
        if (denom > 0.0)
            a = (controlPoints_[rightNeighborID]->pos().x() - pos.x()) / denom;
        color = vec4(a*colorL->r+(1.0-a)*colorR->r,
                     a*colorL->g+(1.0-a)*colorR->g,
                     a*colorL->b+(1.0-a)*colorR->b,
                     pos.y()/height());
    }
    addControlPoint(pos, color);
}

void TransferFunctionEditor::addControlPoint(QPointF pos, vec4 color) {
    ivwAssert(pos.x()>=0.0 && pos.x()<width(), "Transfer function control point's x coordinate out of range.");
    ivwAssert(pos.y()>=0.0 && pos.y()<height(), "Transfer function control point's y coordinate out of range.");

    // add control point to editor and transfer function
    TransferFunctionDataPoint* dataPoint = new TransferFunctionDataPoint(vec2(pos.x()/width(), pos.y()/height()), color);
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
    if (controlPoints_.size() > 1) {
        TransferFunctionEditorLineItem* firstLine = new TransferFunctionEditorLineItem(QPointF(0.0,controlPoints_[0]->pos().y()), controlPoints_[0]->pos());
        lines_.push_back(firstLine);
        addItem(firstLine);
        for (size_t i=0; i<controlPoints_.size()-1; i++) {
            TransferFunctionEditorLineItem* lineGraphicsItem = new TransferFunctionEditorLineItem(controlPoints_[i]->pos(), controlPoints_[i+1]->pos());
            lines_.push_back(lineGraphicsItem);
            addItem(lineGraphicsItem);
        }
        TransferFunctionEditorLineItem* lastLine = new TransferFunctionEditorLineItem(controlPoints_[controlPoints_.size()-1]->pos(),
                                                                                      QPointF(width(), controlPoints_[controlPoints_.size()-1]->pos().y()));
        lines_.push_back(lastLine);
        addItem(lastLine);
    }
}

void TransferFunctionEditor::updateControlPointView() {
    // update lines and repaint
    updateLines();
    update();
    // initiate transfer function update
    emit controlPointsChanged();
}

TransferFunctionEditorControlPoint* TransferFunctionEditor::getControlPointGraphicsItemAt(const QPointF pos) const {
    QList<QGraphicsItem*> graphicsItems = items(pos);
    for (int i=0; i<graphicsItems.size(); i++) {
        TransferFunctionEditorControlPoint* controlPointGraphicsItem = qgraphicsitem_cast<TransferFunctionEditorControlPoint*>(graphicsItems[i]);
        if (controlPointGraphicsItem)
            return controlPointGraphicsItem;
    }
    return 0;
}

}