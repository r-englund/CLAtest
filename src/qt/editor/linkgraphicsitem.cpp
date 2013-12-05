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
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPainterPath>

#include <inviwo/qt/editor/linkgraphicsitem.h>

namespace inviwo {

LinkGraphicsItem::LinkGraphicsItem(QPointF startPoint, QPointF endPoint, ivec3 color)
                                     : startPoint_(startPoint),
                                       endPoint_(endPoint),
                                       color_(color.r, color.g, color.b) {
    setZValue(LINKGRAPHICSITEM_DEPTH);

    QGraphicsDropShadowEffect* processorShadowEffect = new QGraphicsDropShadowEffect();
    processorShadowEffect->setOffset(3.0);
    processorShadowEffect->setBlurRadius(3.0);    
    setGraphicsEffect(processorShadowEffect);
}

LinkGraphicsItem::~LinkGraphicsItem() {}

QPainterPath LinkGraphicsItem::obtainCurvePath() const {
    float delta = endPoint_.y()-startPoint_.y();
        
    QPointF ctrlPoint1 = QPointF((startPoint_.x()+endPoint_.x())/2.0, startPoint_.y());
    QPointF ctrlPoint2 = QPointF(endPoint_.x(), startPoint_.y()+delta/2.0);

    QPainterPath bezierCurve;
    bezierCurve.moveTo(startPoint_);
    bezierCurve.cubicTo(ctrlPoint1, ctrlPoint2, endPoint_);
    return bezierCurve;
}

void LinkGraphicsItem::paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget) {
    IVW_UNUSED_PARAM(options);
    IVW_UNUSED_PARAM(widget);

    if (isSelected()) 
        p->setPen(QPen(Qt::darkRed, 3.0, Qt::DotLine, Qt::RoundCap));        
    else
        p->setPen(QPen(Qt::black, 2.0, Qt::DotLine, Qt::RoundCap));

    p->drawPath(obtainCurvePath());

    p->setPen(QPen(color_, 2.0, Qt::DotLine, Qt::RoundCap));
    p->drawPath(obtainCurvePath());
}

QPainterPath LinkGraphicsItem::shape() const {
    QPainterPathStroker pathStrocker;
    pathStrocker.setWidth(10.0);
    return pathStrocker.createStroke(obtainCurvePath());
}

QRectF LinkGraphicsItem::boundingRect() const {
    QPointF topLeft = QPointF(std::min(startPoint_.x(), endPoint_.x()),
                              std::min(startPoint_.y(), endPoint_.y()));
    return QRectF(topLeft.x()-30.0, topLeft.y()-30.0,
                  abs(startPoint_.x()-endPoint_.x())+60.0,
                  abs(startPoint_.y()-endPoint_.y())+60.0);
}



LinkConnectionGraphicsItem::LinkConnectionGraphicsItem(ProcessorGraphicsItem* outProcessor,
                                               ProcessorGraphicsItem* inProcessor)
                                               : LinkGraphicsItem(outProcessor->getShortestBoundaryPointTo(inProcessor),
                                                                  inProcessor->getShortestBoundaryPointTo(outProcessor), 
                                                                  ivec3(255,255,255)),
                                                 outProcessor_(outProcessor), inProcessor_(inProcessor) {
    setFlags(ItemIsSelectable | ItemIsFocusable);
}

LinkConnectionGraphicsItem::~LinkConnectionGraphicsItem() {}

} // namespace