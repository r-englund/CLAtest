/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Main file author: Timo Ropinski
 *
 *********************************************************************************/

#include <inviwo/qt/widgets/properties/transferfunctioneditor.h>

namespace inviwo {

TransferFunctionEditor::TransferFunctionEditor(TransferFunction* transferFunction, QGraphicsView* view)
    : QGraphicsScene()
    , zoomRangeXMin_(0.0)
    , zoomRangeXMax_(1.0)
    , zoomRangeYMin_(0.0)
    , zoomRangeYMax_(1.0)
    , view_(view)
    , transferFunction_(transferFunction) {
    setSceneRect(0.0, 0.0, 512.0, 512.0);
    // initialize graphicsitem for path through control points
    graphicsPathItem_ = new QGraphicsPathItem();
    QPen pathPen(QColor(66,66,66));
    pathPen.setWidth(3.0);
    pathPen.setCosmetic(true);
    graphicsPathItem_->setPen(pathPen);
    addItem(graphicsPathItem_);
    mouseDrag_ = false;
    // initialize editor with current tf
    recalculateControlPoints();
}

TransferFunctionEditor::~TransferFunctionEditor() {
    for (size_t i=0; i<controlPoints_.size(); i++)
        delete controlPoints_[i];

    controlPoints_.clear();
}

void TransferFunctionEditor::resetTransferFunction() {
    transferFunction_->clearPoints();
    addControlPoint(QPointF(0.0*(width()-1), 0.0*(height()-1)), vec4(0.0f));
    addControlPoint(QPointF(1.0*(width()-1), 1.0*(height()-1)), vec4(1.0f));
}

void TransferFunctionEditor::mousePressEvent(QGraphicsSceneMouseEvent* e) {
    TransferFunctionEditorControlPoint* controlPointGraphicsItem = getControlPointGraphicsItemAt(e->scenePos());

    switch (e->button()) {
        case Qt::LeftButton:
            if (controlPointGraphicsItem) {
                mouseDrag_ = true;
            } else {
                if (e->modifiers() == Qt::NoModifier && QGraphicsScene::selectedItems().isEmpty()) {
                    addControlPoint(e->scenePos());
                    mouseDrag_ = true;
                } else if (e->modifiers() == Qt::ControlModifier) {
                    views().front()->setDragMode(QGraphicsView::RubberBandDrag);
                }
            }

            break;

        case Qt::RightButton:
            if (controlPointGraphicsItem && !mouseDrag_) {
                removeControlPoint(controlPointGraphicsItem);
            }

            break;

        default:
            break;
    }

    QGraphicsScene::mousePressEvent(e);
}

void TransferFunctionEditor::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {
    mouseDrag_ = false;
    QGraphicsScene::mouseReleaseEvent(e);
}

void TransferFunctionEditor::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* e) {
    emit doubleClick();
    QGraphicsScene::mouseDoubleClickEvent(e);
}

void TransferFunctionEditor::keyPressEvent(QKeyEvent* e) {
    // mark all keys
    if (e->key() == 'A' && e->modifiers()==Qt::ControlModifier) {
        QList<QGraphicsItem*> itemList = items();

        for (int i=0; i<itemList.size(); i++) {
            itemList[i]->setSelected(true);
        }
    }

    // delete selected keys
    if (e->key() == Qt::Key_Delete) {
        for (size_t i = 0; i < controlPoints_.size(); i++) {
            if (controlPoints_[i]->isSelected()) {
                removeControlPoint(controlPoints_[i]);
                break; // FIXME can't remove more then one point without crashing.../Peter 140331
            }
        }
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

        vec4 colorL = controlPoints_[leftNeighborID]->getPoint()->getRGBA();
        vec4 colorR = controlPoints_[rightNeighborID]->getPoint()->getRGBA();
        float a = 0.5f;
        float denom = controlPoints_[rightNeighborID]->pos().x() - controlPoints_[leftNeighborID]->pos().x();

        if (denom > 0.0)
            a = (controlPoints_[rightNeighborID]->pos().x() - pos.x()) / denom;

        color = vec4(a*colorL.r+(1.0-a)*colorR.r,
                     a*colorL.g+(1.0-a)*colorR.g,
                     a*colorL.b+(1.0-a)*colorR.b,
                     pos.y()/height());
    }

    addControlPoint(pos, color);
}

void TransferFunctionEditor::addControlPoint(QPointF pos, vec4 color) {
    // add control point to transfer function
    if (pos.x() < 0.0) {
        pos.setX(0.0);
    } else if (pos.x() > width()) {
        pos.setX(width());
    }
    if (pos.y() < 0.0) {
        pos.setY(0.0);
    } else if (pos.y() > height()) {
        pos.setY(height());
    }
    transferFunction_->addPoint(vec2(pos.x()/width(), pos.y()/height()), color);
}

void TransferFunctionEditor::addControlPoint(QPointF pos, TransferFunctionDataPoint* dataPoint) {
    TransferFunctionEditorControlPoint* pointGraphicsItem = new TransferFunctionEditorControlPoint(dataPoint);
    pointGraphicsItem->setPos(pos);
    controlPoints_.push_back(pointGraphicsItem);
    addItem(pointGraphicsItem);
    redrawConnections();
}

void TransferFunctionEditor::removeControlPoint(TransferFunctionEditorControlPoint* controlPoint) {
    if (transferFunction_->getNumDataPoints() > 1)
        transferFunction_->removePoint(controlPoint->getPoint());
}

void TransferFunctionEditor::recalculateControlPoints() {
    if (!mouseDrag_) {
        if (controlPoints_.size() != transferFunction_->getNumDataPoints()) {
            for (size_t i=0; i<controlPoints_.size(); i++) {
                removeItem(controlPoints_[i]);
                delete controlPoints_[i];
            }

            controlPoints_.clear();

            // initialize editor with current tf
            for (size_t i=0; i<transferFunction_->getNumDataPoints(); i++) {
                TransferFunctionDataPoint* dataPoint = transferFunction_->getPoint(static_cast<int>(i));
                vec2 pos = dataPoint->getPos();
                pos.x *= width();
                pos.y *= height();
                // Same code as in addControlPoint. Note: do not use addControlPoint here as it will redraw each time
                TransferFunctionEditorControlPoint* pointGraphicsItem = new TransferFunctionEditorControlPoint(dataPoint);
                pointGraphicsItem->setPos(QPointF(pos.x,pos.y));
                controlPoints_.push_back(pointGraphicsItem);
                addItem(pointGraphicsItem);
            }
        }
    }

    
    redrawConnections();
    update(sceneRect());
}

bool controlPointComparison(TransferFunctionEditorControlPoint* controlPoint0,
                            TransferFunctionEditorControlPoint* controlPoint1) {
    return (controlPoint0->x() < controlPoint1->x());
}

void TransferFunctionEditor::redrawConnections() {
    // sort control point vector
    std::sort(controlPoints_.begin(), controlPoints_.end(), controlPointComparison);
    QPainterPath path;

    if (controlPoints_.size() == 1){
        path.moveTo(QPointF(0.0, controlPoints_[0]->pos().y()));
        path.lineTo(QPointF(width(), controlPoints_[0]->pos().y()));
    } else if (controlPoints_.size() > 1) {
        switch (transferFunction_->getInterpolationType()) {
        case TransferFunction::InterpolationCubic: {
            // draw cubic path
            QPointF p0(QPointF(0.0, controlPoints_[0]->pos().y()));
            QPointF p1(QPointF(controlPoints_[0]->pos().x(), controlPoints_[0]->pos().y()));
            path.moveTo(p0);
            qreal deltaX = p1.x()-p0.x();
            path.cubicTo(QPointF(p0.x()+deltaX/2, p0.y()), QPointF(p0.x()+deltaX/2, p0.y()), p1);

            for (size_t i=0; i<controlPoints_.size()-1; i++) {
                p0 = controlPoints_[i]->pos();
                p1 = controlPoints_[i+1]->pos();
                deltaX = p1.x()-p0.x();
                path.cubicTo(QPointF(p0.x()+deltaX/2, p0.y()), QPointF(p0.x()+deltaX/2, p0.y()), p1);
            }

            // add last path segment
            p0 = QPointF(controlPoints_[controlPoints_.size()-1]->pos().x(),
                         controlPoints_[controlPoints_.size()-1]->pos().y());
            p1 = QPointF(width(), p0.y());
            deltaX = p1.x()-p0.x();
            path.moveTo(p0);
            path.cubicTo(QPointF(p0.x()+deltaX/2, p0.y()), QPointF(p0.x()+deltaX/2, p0.y()), p1);
            break;
            }
        
        default: {
            // draw linear path
            path.moveTo(QPointF(0.0, controlPoints_[0]->pos().y()));

            for (size_t i=0; i<controlPoints_.size(); i++) {
                path.lineTo(QPointF(controlPoints_[i]->pos().x(), controlPoints_[i]->pos().y()));
            }

            // add last path segment
            path.lineTo(QPointF(width(), controlPoints_[controlPoints_.size()-1]->pos().y()));
            break;
            }
        }
    }
    graphicsPathItem_->setPath(path);
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