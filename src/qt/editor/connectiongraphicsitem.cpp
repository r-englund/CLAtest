/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPainterPath>

#include <inviwo/qt/editor/connectiongraphicsitem.h>

namespace inviwo {

CurveGraphicsItem::CurveGraphicsItem(QPointF startPoint, QPointF endPoint, uvec3 color)
    : color_(color.r, color.g, color.b) {
    setStartPoint(startPoint);
    setMidPoint(startPoint);
    setEndPoint(endPoint);
    setZValue(CONNECTIONGRAPHICSITEM_DEPTH);
    hoverInputColor_ = QColor();
    hoverOutputColor_ = QColor();
    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect();
    shadowEffect->setOffset(3.0);
    shadowEffect->setBlurRadius(3.0);
    setGraphicsEffect(shadowEffect);
    resetBorderColors();
}

CurveGraphicsItem::~CurveGraphicsItem() {}

QPainterPath CurveGraphicsItem::obtainCurvePath() const {
    float delta = endPoint_.y()-startPoint_.y();
    QPointF ctrlPoint1 = QPointF(startPoint_.x(), endPoint_.y()-delta/4.0);
    QPointF ctrlPoint2 = QPointF(endPoint_.x(), startPoint_.y()+delta/4.0);
    QPainterPath bezierCurve;
    bezierCurve.moveTo(startPoint_);
    bezierCurve.cubicTo(ctrlPoint1, ctrlPoint2, endPoint_);
    return bezierCurve;
}

QPainterPath CurveGraphicsItem::obtainCurvePath(QPointF startPoint, QPointF endPoint) {
    float delta = endPoint.y()-startPoint.y();
    QPointF ctrlPoint1 = QPointF(startPoint.x(), endPoint.y()-delta/4.0);
    QPointF ctrlPoint2 = QPointF(endPoint.x(), startPoint.y()+delta/4.0);
    QPainterPath bezierCurve;
    bezierCurve.moveTo(startPoint);
    bezierCurve.cubicTo(ctrlPoint1, ctrlPoint2, endPoint);
    return bezierCurve;
}

void CurveGraphicsItem::paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget) {
    IVW_UNUSED_PARAM(options);
    IVW_UNUSED_PARAM(widget);

    if (midPoint_ == startPoint_) {
        if (isSelected())
            p->setPen(QPen(selectedBorderColor_, 4.0, Qt::SolidLine, Qt::RoundCap));
        else
            p->setPen(QPen(borderColor_, 3.0, Qt::SolidLine, Qt::RoundCap));

        p->drawPath(obtainCurvePath());
        p->setPen(QPen(color_, 2.0, Qt::SolidLine, Qt::RoundCap));
        p->drawPath(obtainCurvePath());
    } else if (midPoint_ != startPoint_) {
        p->setPen(QPen(hoverInputColor_, 4.0, Qt::SolidLine, Qt::RoundCap)); //< Shadow
        p->drawPath(obtainCurvePath(startPoint_,midPoint_));
        p->setPen(QPen(color_, 2.0, Qt::SolidLine, Qt::RoundCap));
        p->drawPath(obtainCurvePath(startPoint_,midPoint_));
        p->setPen(QPen(hoverOutputColor_, 4.0, Qt::SolidLine, Qt::RoundCap)); //< Shadow
        p->drawPath(obtainCurvePath(midPoint_, endPoint_));
        p->setPen(QPen(color_, 2.0, Qt::SolidLine, Qt::RoundCap));
        p->drawPath(obtainCurvePath(midPoint_, endPoint_));
    }
}

QPainterPath CurveGraphicsItem::shape() const {
    QPainterPathStroker pathStrocker;
    pathStrocker.setWidth(10.0);
    return pathStrocker.createStroke(obtainCurvePath());
}

void CurveGraphicsItem::resetBorderColors() {
    setBorderColor(Qt::black);
    setSelectedBorderColor(Qt::darkRed);
}

QRectF CurveGraphicsItem::boundingRect() const {
    QPointF topLeft = QPointF(std::min(startPoint_.x(), endPoint_.x()),
                              std::min(startPoint_.y(), endPoint_.y()));
    return QRectF(topLeft.x()-30.0, topLeft.y()-30.0,
                  abs(startPoint_.x()-endPoint_.x())+60.0,
                  abs(startPoint_.y()-endPoint_.y())+60.0);
}



ConnectionGraphicsItem::ConnectionGraphicsItem(ProcessorGraphicsItem* outProcessor, Outport* outport,
        ProcessorGraphicsItem* inProcessor, Inport* inport)
    : CurveGraphicsItem(outProcessor->mapToScene(outProcessor->calculatePortRect(outport)).boundingRect().center(),
                        inProcessor->mapToScene(inProcessor->calculatePortRect(inport)).boundingRect().center(),
                        inport->getColorCode()),
    outProcessor_(outProcessor),
    inProcessor_(inProcessor),
    outport_(outport),
    inport_(inport) {
    setFlags(ItemIsSelectable | ItemIsFocusable);
}

ConnectionGraphicsItem::~ConnectionGraphicsItem() {}

} // namespace