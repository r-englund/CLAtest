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
#include <inviwo/core/datastructures/transferfunction.h>
#include <inviwo/qt/widgets/properties/transferfunctioneditorcontrolpoint.h>
#include <inviwo/qt/widgets/properties/transferfunctioncontrolpointconnection.h>


#include <QBrush>

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QLineF>
#include <QPainter>
#include <QPen>

namespace inviwo {

class ControlPointEquals {
public:
    ControlPointEquals(const TransferFunctionDataPoint* p) : p_(p) {}
    bool operator()(TransferFunctionEditorControlPoint* editorPoint) {
        return editorPoint->getPoint() == p_;
    }
    bool operator<(TransferFunctionEditorControlPoint* editorPoint) {
        return editorPoint->getPoint()->getPos().x < p_->getPos().x;
    }

private:
    const TransferFunctionDataPoint* p_;
};

TransferFunctionEditor::TransferFunctionEditor(TransferFunction* transferFunction,
                                               QGraphicsView* view)
    : QGraphicsScene()
    , zoomRangeXMin_(0.0)
    , zoomRangeXMax_(1.0)
    , zoomRangeYMin_(0.0)
    , zoomRangeYMax_(1.0)
    , view_(view)
    , transferFunction_(transferFunction) {
    setSceneRect(0.0, 0.0, 512.0, 512.0);
    mouseDrag_ = false;
    // initialize editor with current tf
    
    // The defalt bsp tends to crash...  
    setItemIndexMethod(QGraphicsScene::NoIndex);
    
    for (int i = 0; i < transferFunction_->getNumPoints(); ++i){
        onControlPointAdded(transferFunction_->getPoint(i));
    }
}

TransferFunctionEditor::~TransferFunctionEditor() {
    for (size_t i = 0; i < points_.size(); i++) delete points_[i];
    points_.clear();
    
    for (size_t i = 0; i < connections_.size(); i++) delete connections_[i];
    connections_.clear();
}

void TransferFunctionEditor::resetTransferFunction() {
    transferFunction_->clearPoints();
    addControlPoint(QPointF(0.0 * (width() - 1), 0.0 * (height() - 1)), vec4(0.0f));
    addControlPoint(QPointF(1.0 * (width() - 1), 1.0 * (height() - 1)), vec4(1.0f));
}

void TransferFunctionEditor::mousePressEvent(QGraphicsSceneMouseEvent* e) {
    TransferFunctionEditorControlPoint* controlPointGraphicsItem =
        getControlPointGraphicsItemAt(e->scenePos());

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

void TransferFunctionEditor::mouseMoveEvent(QGraphicsSceneMouseEvent* e){
    if(mouseDrag_) InviwoApplication::getPtr()->getProcessorNetwork()->lock();
    QGraphicsScene::mouseMoveEvent(e);
    if(mouseDrag_) InviwoApplication::getPtr()->getProcessorNetwork()->unlock();
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
    InviwoApplication::getPtr()->getProcessorNetwork()->lock();
    if (e->key() == 'A' && e->modifiers() == Qt::ControlModifier) {  // Select all
        QList<QGraphicsItem*> itemList = items();
        for (int i = 0; i < itemList.size(); i++) {
            itemList[i]->setSelected(true);
        }

    } else if (e->key() == 'D' && e->modifiers() == Qt::ControlModifier) {  // Select none
        QList<QGraphicsItem*> itemList = selectedItems();
        for (int i = 0; i < itemList.size(); i++) {
            itemList[i]->setSelected(false);
        }

    } else if (e->key() == Qt::Key_Delete) {  // Delete selected
        QList<QGraphicsItem*> itemList = selectedItems();
        for (int i = 0; i < itemList.size(); i++) {
            TransferFunctionEditorControlPoint* p =
                qgraphicsitem_cast<TransferFunctionEditorControlPoint*>(itemList[i]);
            if (p) {
                removeControlPoint(p);
            }
        }

    } else if (e->modifiers() != Qt::ControlModifier &&  // Move points
               (e->key() == Qt::Key_Left || e->key() == Qt::Key_Right || e->key() == Qt::Key_Up ||
                e->key() == Qt::Key_Down || e->key() == 'I' || e->key() == 'J' || e->key() == 'K' ||
                e->key() == 'L')) {
        QPointF delta;
        switch (e->key()) {
            case Qt::Key_Left:
                delta = QPointF(-1.0f, 0.0f);
                break;
            case Qt::Key_Right:
                delta = QPointF(1.0f, 0.0f);
                break;
            case Qt::Key_Up:
                delta = QPointF(0.0f, 1.0f);
                break;
            case Qt::Key_Down:
                delta = QPointF(0.0f, -1.0f);
                break;

            case 'J':
                delta = QPointF(-1.0f, 0.0f);
                break;
            case 'L':
                delta = QPointF(1.0f, 0.0f);
                break;
            case 'I':
                delta = QPointF(0.0f, 1.0f);
                break;
            case 'K':
                delta = QPointF(0.0f, -1.0f);
                break;
        }

        if (e->modifiers() == Qt::ShiftModifier || e->modifiers() == Qt::AltModifier) {
            delta *= 10.0;
        }

        QList<QGraphicsItem*> selitems = selectedItems();
        for (int i = 0; i < selitems.size(); i++) {
            selitems[i]->setPos(selitems[i]->pos() + delta);
        }

    } else if (e->modifiers() == Qt::ControlModifier &&  // Modify selection
               (e->key() == 'I' || e->key() == 'J' || e->key() == 'K' || e->key() == 'L')) {
        QList<QGraphicsItem*> selitems = selectedItems();

        std::vector<TransferFunctionEditorControlPoint*> points;
        for (int i = 0; i < selitems.size(); i++) {
            TransferFunctionEditorControlPoint* p =
                qgraphicsitem_cast<TransferFunctionEditorControlPoint*>(selitems[i]);
            if (p) points.push_back(p);
        }
        std::stable_sort(points.begin(), points.end(),
                         comparePtr<TransferFunctionEditorControlPoint>);

        switch (e->key()) {
            case 'J':
                if (points.size() > 0) {
                    if (points.front()->left_ && points.front()->left_->left_) {
                        points.front()->left_->left_->setSelected(true);
                    }
                } else if (points_.size() > 0) {
                    points_.back()->setSelected(true);
                }

                break;
            case 'L':
                if (points.size() > 0) {
                    if (points.back()->right_ && points.back()->right_->right_) {
                        points.back()->right_->right_->setSelected(true);
                    }
                } else if (points_.size() > 0) {
                    points_.front()->setSelected(true);
                }
                break;
            case 'I':
                if (points.size() > 0) {
                    points.front()->setSelected(false);
                }
                break;
            case 'K':
                if (points.size() > 0) {
                    points.back()->setSelected(false);
                }
                break;
        }
    } else {
        e->ignore();
    }
    InviwoApplication::getPtr()->getProcessorNetwork()->unlock();

    QGraphicsScene::keyPressEvent(e);
}

void TransferFunctionEditor::addControlPoint(QPointF pos) {
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
    transferFunction_->addPoint(vec2(pos.x() / width(), pos.y() / height()));
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
    transferFunction_->addPoint(vec2(pos.x() / width(), pos.y() / height()), color);
}

void TransferFunctionEditor::removeControlPoint(TransferFunctionEditorControlPoint* controlPoint) {
    if (transferFunction_->getNumPoints() > 1) {
        transferFunction_->removePoint(controlPoint->getPoint());
    }
}

TransferFunctionEditorControlPoint* TransferFunctionEditor::getControlPointGraphicsItemAt(
    const QPointF pos) const {
    QList<QGraphicsItem*> graphicsItems = items(pos);

    for (int i = 0; i < graphicsItems.size(); i++) {
        TransferFunctionEditorControlPoint* controlPointGraphicsItem =
            qgraphicsitem_cast<TransferFunctionEditorControlPoint*>(graphicsItems[i]);

        if (controlPointGraphicsItem) return controlPointGraphicsItem;
    }

    return 0;
}

void TransferFunctionEditor::onControlPointAdded(TransferFunctionDataPoint* p) {
    QPointF pos(p->getPos().x * width(), p->getPos().y * height());

    TransferFunctionEditorControlPoint* newpoint =
        new TransferFunctionEditorControlPoint(p, dataMap_);
    newpoint->setPos(pos);

    PointVec::iterator it = std::lower_bound(points_.begin(), points_.end(), newpoint,
                                             comparePtr<TransferFunctionEditorControlPoint>);
    it = points_.insert(it, newpoint);

    updateConnections();

    addItem(newpoint);
}

void TransferFunctionEditor::onControlPointRemoved(TransferFunctionDataPoint* p) {
    std::vector<TransferFunctionEditorControlPoint*>::iterator it =
        std::find_if(points_.begin(), points_.end(), ControlPointEquals(p));
    if (it != points_.end()) {
        removeItem((*it));
        delete *it;
        points_.erase(it);
        
        updateConnections();
    }
}

void TransferFunctionEditor::updateConnections() {
    std::stable_sort(points_.begin(), points_.end(), comparePtr<TransferFunctionEditorControlPoint>);
    
    while (connections_.size() < points_.size() + 1){
        TransferFunctionControlPointConnection* c = new TransferFunctionControlPointConnection();
        connections_.push_back(c);
        addItem(c);
    }
    while (connections_.size() > points_.size() + 1){
        TransferFunctionControlPointConnection* c = connections_.back();
        removeItem(c);
        delete c;
        connections_.pop_back();
    }
    
    connections_[0]->left_ = NULL;
    connections_[connections_.size()-1]->right_ = NULL;
    
    for (int i = 0; i < points_.size(); ++i){
        points_[i]->left_ = connections_[i];
        points_[i]->right_ = connections_[i+1];
        connections_[i]->right_ = points_[i];
        connections_[i+1]->left_ = points_[i];
    }
    
    for (int i = 0; i < connections_.size(); ++i){
        connections_[i]->updateShape();
    }
}

void TransferFunctionEditor::onControlPointChanged(const TransferFunctionDataPoint* p) {
}

void TransferFunctionEditor::setDataMap(const DataMapper& dataMap) {
   dataMap_ = dataMap;
}

inviwo::DataMapper TransferFunctionEditor::getDataMap() const {
    return dataMap_;
}

float TransferFunctionEditor::getZoomRangeXMin() const {
    return zoomRangeXMin_;
}

void TransferFunctionEditor::setZoomRangeXMin(float min) {
    zoomRangeXMin_ = min;
}

float TransferFunctionEditor::getZoomRangeXMax() const {
    return zoomRangeXMax_;
}

void TransferFunctionEditor::setZoomRangeXMax(float max) {
    zoomRangeXMax_ = max;
}

float TransferFunctionEditor::getZoomRangeYMin() const {
    return zoomRangeYMin_;
}

void TransferFunctionEditor::setZoomRangeYMin(float min) {
    zoomRangeYMin_ = min;
}

float TransferFunctionEditor::getZoomRangeYMax() const {
    return zoomRangeYMax_;
}

void TransferFunctionEditor::setZoomRangeYMax(float max) {
    zoomRangeYMax_ = max;
}

QGraphicsView* TransferFunctionEditor::getView() {
    return view_;
}

}