/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2014 Inviwo Foundation
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
 * Main file author: Erik Sundén
 *
 *********************************************************************************/

#include <QApplication>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QVector2D>

#include <inviwo/qt/editor/networkeditor.h>
#include <inviwo/qt/editor/portinfographicsitem.h>

namespace inviwo {

static const int width = 150;
static const int height = 50;
static const int roundedCorners = 9;
static const int labelHeight = 8;

int pointSizeToPixelSizePI(const int pointSize) {
    // compute pixel size for fonts by assuming 96 dpi as basis
    return ((pointSize * 4) / 3);
}

PortInfoGraphicsItem::PortInfoGraphicsItem()
    : ProcessorObserver(), LabelGraphicsItemObserver(), processor_(NULL) {
    setZValue(PROCESSORGRAPHICSITEM_DEPTH);
    setFlags(ItemIsMovable | ItemIsSelectable | ItemIsFocusable | ItemSendsGeometryChanges);
    setRect(-width/2, -height/2, width, height);
    QGraphicsDropShadowEffect* processorShadowEffect = new QGraphicsDropShadowEffect();
    processorShadowEffect->setOffset(3.0);
    processorShadowEffect->setBlurRadius(3.0);
    setGraphicsEffect(processorShadowEffect);
    nameLabel_ = new LabelGraphicsItem(this);
    nameLabel_->setCrop(9, 8);
    nameLabel_->setPos(-width/2.0+labelHeight, -height/2.0+labelHeight);
    nameLabel_->setDefaultTextColor(Qt::white);
    QFont nameFont("Segoe", labelHeight, QFont::Black, false);
    nameFont.setPixelSize(pointSizeToPixelSizePI(labelHeight));
    nameLabel_->setFont(nameFont);
    LabelGraphicsItemObserver::addObservation(nameLabel_);
    classLabel_ = new LabelGraphicsItem(this);
    classLabel_->setCrop(9, 8);
    classLabel_->setPos(-width/2.0+labelHeight, -height/2.0+labelHeight*2.5);
    classLabel_->setDefaultTextColor(Qt::lightGray);
    QFont classFont("Segoe", labelHeight, QFont::Normal, true);
    classFont.setPixelSize(pointSizeToPixelSizePI(labelHeight));
    classLabel_->setFont(classFont);
}

PortInfoGraphicsItem::~PortInfoGraphicsItem() {}

void PortInfoGraphicsItem::paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget) {
    IVW_UNUSED_PARAM(options);
    IVW_UNUSED_PARAM(widget);
    p->save();
    p->setPen(Qt::NoPen);
    p->setRenderHint(QPainter::Antialiasing, true);
    QColor topColor(140,140,140);
    QColor middleColor(59,61,61);
    QColor bottomColor(40,40,40);
    // paint processor
    QLinearGradient grad(rect().topLeft(), rect().bottomLeft());

    if (isSelected()) {
        grad.setColorAt(0.0f, topColor);
        grad.setColorAt(0.2f, middleColor);
        grad.setColorAt(0.5f, Qt::darkRed);
        grad.setColorAt(1.0f, bottomColor);
    } else {
        grad.setColorAt(0.0f, topColor);
        grad.setColorAt(0.2f, middleColor);
        grad.setColorAt(1.0f, bottomColor);
    }

    QRectF bRect = rect();
    QPainterPath roundRectPath;
    roundRectPath.moveTo(bRect.left(), bRect.top()+roundedCorners);
    roundRectPath.lineTo(bRect.left(), bRect.bottom()-roundedCorners);
    roundRectPath.arcTo(bRect.left(), bRect.bottom()-(2*roundedCorners), (2*roundedCorners), (2*roundedCorners), 180.0, 90.0);
    roundRectPath.lineTo(bRect.right()-roundedCorners, bRect.bottom());
    roundRectPath.arcTo(bRect.right()-(2*roundedCorners), bRect.bottom()-(2*roundedCorners), (2*roundedCorners), (2*roundedCorners), 270.0,
                        90.0);
    roundRectPath.lineTo(bRect.right(), bRect.top()+roundedCorners);
    roundRectPath.arcTo(bRect.right()-(2*roundedCorners), bRect.top(), (2*roundedCorners), (2*roundedCorners), 0.0, 90.0);
    roundRectPath.lineTo(bRect.left()+roundedCorners, bRect.top());
    roundRectPath.arcTo(bRect.left(), bRect.top(), (2*roundedCorners), (2*roundedCorners), 90.0, 90.0);
    p->setBrush(grad);
    p->drawPath(roundRectPath);
    QLinearGradient highlightGrad(rect().topLeft(), rect().bottomLeft());

    if (isSelected()) {
        highlightGrad.setColorAt(0.0f, bottomColor);
        highlightGrad.setColorAt(0.1f, bottomColor);
        highlightGrad.setColorAt(0.5f, Qt::darkRed);
        highlightGrad.setColorAt(1.0f, bottomColor);
    } else {
        highlightGrad.setColorAt(0.0f, bottomColor);
        highlightGrad.setColorAt(1.0f, bottomColor);
    }

    QPainterPath highlightPath;
    float highlightLength = bRect.width()/8.0;
    highlightPath.moveTo(bRect.left(), bRect.top()+roundedCorners);
    highlightPath.lineTo(bRect.left(), bRect.bottom()-roundedCorners);
    highlightPath.arcTo(bRect.left(), bRect.bottom()-(2*roundedCorners), (2*roundedCorners), (2*roundedCorners), 180.0, 90.0);
    highlightPath.lineTo(bRect.left()+(bRect.width()/2.0)+highlightLength, bRect.bottom());
    highlightPath.lineTo(bRect.left()+(bRect.width()/2.0)-highlightLength, bRect.top());
    highlightPath.lineTo(bRect.left()+roundedCorners, bRect.top());
    highlightPath.arcTo(bRect.left(), bRect.top(), (2*roundedCorners), (2*roundedCorners), 90.0, 90.0);
    p->setBrush(highlightGrad);
    p->drawPath(highlightPath);
    p->setPen(QPen(QColor(164,164,164), 1.0));
    p->setBrush(Qt::NoBrush);
    p->drawPath(roundRectPath);
}

void PortInfoGraphicsItem::updateViews() {
    foreach(QGraphicsView *view, this->scene()->views()) {
        view->viewport()->update();
    }
    qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
}

} // namespace
