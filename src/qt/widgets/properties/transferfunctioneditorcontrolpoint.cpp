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

#include <inviwo/core/datastructures/transferfunctiondatapoint.h>
#include <inviwo/qt/widgets/properties/transferfunctioneditorcontrolpoint.h>
#include <inviwo/qt/widgets/properties/transferfunctioneditor.h>
#include <inviwo/qt/widgets/properties/transferfunctionpropertydialog.h>

namespace inviwo {

TransferFunctionEditorControlPoint::TransferFunctionEditorControlPoint(TransferFunctionDataPoint* datapoint)
    : datapoint_(datapoint) {

    setFlags(ItemIsMovable | ItemIsSelectable | ItemIsFocusable | ItemSendsGeometryChanges);
    //setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
    viewWidth_ = 255.0;
    viewHeight_ = 100.0;

    setPos(datapoint_->getPos()->x * viewWidth_, datapoint_->getPos()->y * viewHeight_);
    setZValue(1);
    size_ = 12.0f;
    leftNeighbour_ = NULL;
    rightNeighbour_ = NULL;
}

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
    return QRectF(-bboxsize/2.0, -bboxsize/2.0f, bboxsize, bboxsize);
}

/*
void TransferFunctionEditorControlPoint::mouseMoveEvent(QGraphicsSceneMouseEvent * e) {
    viewWidth_ = static_cast<TransferFunctionEditor*>(scene())->getView()->width();
    viewHeight_ = static_cast<TransferFunctionEditor*>(scene())->getView()->height();

    float zoomRangeMin = static_cast<TransferFunctionEditor*>(scene())->getZoomRangeXMin();
    float zoomRangeMax = static_cast<TransferFunctionEditor*>(scene())->getZoomRangeXMax();

    vec2 pos = vec2(e->scenePos().x(), e->scenePos().y());

    float minX = (getLeftNeighbour()) ? getLeftNeighbour()->x() : 0.0f;
    float maxX = (getRightNeighbour()) ? getRightNeighbour()->x() : viewWidth_;
    float minY = 0.0;
    float maxY = viewHeight_;

    pos.x = (pos.x <= minX) ? minX + 0.001f : pos.x;
    pos.x = (pos.x >= maxX) ? maxX - 0.001f : pos.x;
    pos.y = (pos.y <= minY) ? minY : pos.y;
    pos.y = (pos.y >= maxY) ? maxY : pos.y;

    float mappedXpos = pos.x / viewWidth_ * (zoomRangeMax - zoomRangeMin) + zoomRangeMin;

    setPos(QPointF(pos.x, pos.y));
    datapoint_->setPos(mappedXpos, pos.y / viewHeight_);
    datapoint_->setA(pos.y/viewHeight_);
}
*/

TransferFunctionDataPoint* TransferFunctionEditorControlPoint::getPoint() const {return datapoint_;}

TransferFunctionEditorControlPoint* TransferFunctionEditorControlPoint::getLeftNeighbour() const{return leftNeighbour_;}

TransferFunctionEditorControlPoint* TransferFunctionEditorControlPoint::getRightNeighbour() const{return rightNeighbour_;}

void TransferFunctionEditorControlPoint::setLeftNeighbour(TransferFunctionEditorControlPoint* point){leftNeighbour_ = point;}

void TransferFunctionEditorControlPoint::setRightNeighbour(TransferFunctionEditorControlPoint* point){rightNeighbour_ = point;}

QVariant TransferFunctionEditorControlPoint::itemChange(GraphicsItemChange change, const QVariant &value){
    if (change == QGraphicsItem::ItemPositionChange && scene()) {
        // constrain positions to valid view positions
        QPointF newPos = value.toPointF();
        viewWidth_ = static_cast<TransferFunctionEditor*>(scene())->getView()->width();
        viewHeight_ = static_cast<TransferFunctionEditor*>(scene())->getView()->height();
        QRectF rect = QRectF(0.0,0.0,viewWidth_,viewHeight_);
        if (!rect.contains(newPos)) {
            newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
            newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
        }
        // update the associated transfer function data point
        QPointF controlPointPos = pos();
        datapoint_->setPos(controlPointPos.x()/viewWidth_, controlPointPos.y()/viewHeight_);
        datapoint_->setA(controlPointPos.y()/viewHeight_);
        // let the editor know that an update is required
        static_cast<TransferFunctionEditor*>(scene())->updateControlPointView();
        // return the constraint position
        return newPos;
        /*

        float zoomRangeMin = static_cast<TransferFunctionEditor*>(scene())->getZoomRangeXMin();
        float zoomRangeMax = static_cast<TransferFunctionEditor*>(scene())->getZoomRangeXMax();

        vec2 pos = vec2(e->scenePos().x(), e->scenePos().y());

        float minX = (getLeftNeighbour()) ? getLeftNeighbour()->x() : 0.0f;
        float maxX = (getRightNeighbour()) ? getRightNeighbour()->x() : viewWidth_;
        float minY = 0.0;
        float maxY = viewHeight_;

        pos.x = (pos.x <= minX) ? minX + 0.001f : pos.x;
        pos.x = (pos.x >= maxX) ? maxX - 0.001f : pos.x;
        pos.y = (pos.y <= minY) ? minY : pos.y;
        pos.y = (pos.y >= maxY) ? maxY : pos.y;

        float mappedXpos = pos.x / viewWidth_ * (zoomRangeMax - zoomRangeMin) + zoomRangeMin;

        setPos(QPointF(pos.x, pos.y));
        datapoint_->setPos(mappedXpos, pos.y / viewHeight_);
        datapoint_->setA(pos.y/viewHeight_);
*/
    }

    if (change == QGraphicsItem::ItemSelectedChange){
        //datapoint_->setSelected(!isSelected());
    }
    return QGraphicsItem::itemChange(change, value);
}

} // namespace