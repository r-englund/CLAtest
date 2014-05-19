/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
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
 * Main file authors: Timo Ropinski, Hans-Christian Helltegen
 *
 *********************************************************************************/

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
    return obtainCurvePath(startPoint_,endPoint_);
}

QPainterPath CurveGraphicsItem::obtainCurvePath(QPointF startPoint, QPointF endPoint) const {
    float delta =  std::abs(endPoint.y()-startPoint.y());
    
    QPointF o = (endPoint-startPoint);
    o -= QPointF(0,9); //take into acount port size
    int min = std::sqrtf(QPointF::dotProduct(o,o));
    static const int max = 50;
    if(delta < min) delta = min;
    if(delta > max) delta = max;
    
    QPointF off(0,delta);
    QPointF ctrlPoint1 = startPoint_ + off;
    QPointF ctrlPoint2 = endPoint_ - off;
    QPainterPath bezierCurve;
    bezierCurve.moveTo(startPoint);
    bezierCurve.cubicTo(ctrlPoint1, ctrlPoint2, endPoint);
    return bezierCurve;
}

void CurveGraphicsItem::paint(QPainter* p, const QStyleOptionGraphicsItem* options,
                              QWidget* widget) {
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
        p->setPen(QPen(hoverInputColor_, 4.0, Qt::SolidLine, Qt::RoundCap));  //< Shadow
        p->drawPath(obtainCurvePath(startPoint_, midPoint_));
        p->setPen(QPen(color_, 2.0, Qt::SolidLine, Qt::RoundCap));
        p->drawPath(obtainCurvePath(startPoint_, midPoint_));
        p->setPen(QPen(hoverOutputColor_, 4.0, Qt::SolidLine, Qt::RoundCap));  //< Shadow
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
    QRectF p = obtainCurvePath().boundingRect();
    return QRectF(p.topLeft() - QPointF(5,5) , p.size() + QSizeF(10,10));
}

ConnectionGraphicsItem::ConnectionGraphicsItem(ProcessorGraphicsItem* outProcessor,
                                               Outport* outport, ProcessorGraphicsItem* inProcessor,
                                               Inport* inport)
    : CurveGraphicsItem(
          outProcessor->mapToScene(outProcessor->calculatePortRect(outport))
              .boundingRect()
              .center(),
          inProcessor->mapToScene(inProcessor->calculatePortRect(inport)).boundingRect().center(),
          inport->getColorCode())
    , outProcessor_(outProcessor)
    , inProcessor_(inProcessor)
    , outport_(outport)
    , inport_(inport) {
    setFlags(ItemIsSelectable | ItemIsFocusable);
}

ConnectionGraphicsItem::~ConnectionGraphicsItem() {}

}  // namespace