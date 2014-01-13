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

#include <inviwo/qt/widgets/properties/transferfunctioneditorlineitem.h>
#include <inviwo/core/common/inviwo.h>
#include <QPainter>
#include <math.h>

namespace inviwo {

TransferFunctionEditorLineItem::TransferFunctionEditorLineItem(QPointF startPos, QPointF endPos):
    startPos_(startPos),
    endPos_(endPos)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
    setFlag(QGraphicsItem::ItemIgnoresTransformations, false);
    setZValue(0);
}

TransferFunctionEditorLineItem::~TransferFunctionEditorLineItem() {}

void TransferFunctionEditorLineItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget* widget) {
    IVW_UNUSED_PARAM(options);
    IVW_UNUSED_PARAM(widget);
    painter->setRenderHint(QPainter::Antialiasing, true);
    QPen pen = QPen();
    pen.setCosmetic(true);
    pen.setStyle(Qt::SolidLine);
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidth(2.0);
    isSelected() ? pen.setColor(Qt::red) : pen.setColor(Qt::black);
    painter->setPen(pen);
    painter->drawLine(startPos_, endPos_);
}

QPainterPath TransferFunctionEditorLineItem::shape() const {
    QPointF delta = endPos_ - startPos_;
    float angle = atan2(delta.y(), delta.x());
    float boxWidth = 20.0f;
    QPointF rotatedSize = QPointF(sin(angle), -cos(angle)) * boxWidth/2;

    QPolygonF boundingPolygon;
    boundingPolygon	<< startPos_ - rotatedSize	// lower left
                    << startPos_ + rotatedSize	// upper left
                    << endPos_ + rotatedSize	// upper right
                    << endPos_ - rotatedSize;	// lower right

    QPainterPath boundingPath;
    boundingPath.addPolygon(boundingPolygon);
    boundingPath.closeSubpath();
    return boundingPath;
}

QRectF TransferFunctionEditorLineItem::boundingRect() const {
    return shape().boundingRect();
}

/*
void TransferFunctionEditorLineItem::mouseMoveEvent(QGraphicsSceneMouseEvent* e){
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

    start_->setPos(startPos);
    finish_->setPos(finishPos);

    float zoomRangeMin = static_cast<TransferFunctionEditor*>(this->scene())->getZoomRangeXMin();
    float zoomRangeMax = static_cast<TransferFunctionEditor*>(this->scene())->getZoomRangeXMax();

    float mappedStartXpos = startPos.x() / viewWidth * (zoomRangeMax - zoomRangeMin) + zoomRangeMin;
    float mappedFinishXpos = finishPos.x() / viewWidth * (zoomRangeMax - zoomRangeMin) + zoomRangeMin;

    start_->getPoint()->setPos(vec2(mappedStartXpos, startPos.y()/viewHeight));
    finish_->getPoint()->setPos(vec2(mappedFinishXpos, finishPos.y()/viewHeight));

    mouseDownPos_ = e->pos();
}
*/

} // namespace
