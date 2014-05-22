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
 * Main file authors: Timo Ropinski, Viktor Axelsson
 *
 *********************************************************************************/

#include <inviwo/qt/widgets/properties/transferfunctioneditorcontrolpoint.h>
#include <inviwo/core/datastructures/transferfunctiondatapoint.h>
#include <inviwo/qt/widgets/properties/transferfunctioneditor.h>
#include <inviwo/qt/widgets/properties/transferfunctioneditorview.h>
#include <QTextStream>

namespace inviwo {

TransferFunctionEditorControlPoint::TransferFunctionEditorControlPoint(
    TransferFunctionDataPoint* datapoint, const DataMapper& dataMap)
    : isEditingPoint_(false), dataPoint_(datapoint), dataMap_(dataMap) {
    size_ = 14.0f;
    showLabel_ = false;
    setFlags(ItemIgnoresTransformations | ItemIsFocusable | ItemIsMovable | ItemIsSelectable |
             ItemSendsGeometryChanges);
    setZValue(1);
    setAcceptHoverEvents(true);
    datapoint->addObserver(this);
}

TransferFunctionEditorControlPoint::~TransferFunctionEditorControlPoint() {}

void TransferFunctionEditorControlPoint::paint(QPainter* painter,
                                               const QStyleOptionGraphicsItem* options,
                                               QWidget* widget) {
    IVW_UNUSED_PARAM(options);
    IVW_UNUSED_PARAM(widget);
    painter->setRenderHint(QPainter::Antialiasing, true);
    QPen pen = QPen();
    pen.setWidth(3);
    pen.setCosmetic(true);
    pen.setCapStyle(Qt::RoundCap);
    pen.setStyle(Qt::SolidLine);
    isSelected() ? pen.setColor(QColor(213, 79, 79)) : pen.setColor(QColor(66, 66, 66));
    QBrush brush = QBrush(QColor::fromRgbF(dataPoint_->getRGBA().r, dataPoint_->getRGBA().g,
                                           dataPoint_->getRGBA().b));
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawEllipse(-size_ / 2.0, -size_ / 2.0, size_, size_);

    if (showLabel_) {
        QString label;
        QTextStream labelStream(&label);
        labelStream.setRealNumberPrecision(3);
        labelStream << "a("
                    << dataMap_.dataRange.x +
                           dataPoint_->getPos().x * (dataMap_.dataRange.y - dataMap_.dataRange.x)
                    << ")=";
        labelStream << dataPoint_->getRGBA().a;

        Qt::AlignmentFlag align;
        if (dataPoint_->getPos().x > 0.5f) {
            align = Qt::AlignRight;
        } else {
            align = Qt::AlignLeft;
        }
        QRectF rect = calculateLabelRect();

        pen.setColor(QColor(46, 46, 46));
        painter->setPen(pen);
        QFont font;
        font.setPixelSize(14);
        painter->setFont(font);
        painter->drawText(rect, align, label);
    }
}

QRectF TransferFunctionEditorControlPoint::boundingRect() const {
    float bBoxSize = size_ + 30.0f;
    if (showLabel_) {
        QRectF rect = calculateLabelRect();
        return rect.united(QRectF(-bBoxSize / 2.0, -bBoxSize / 2.0f, bBoxSize, bBoxSize));
    } else {
        return QRectF(-bBoxSize / 2.0, -bBoxSize / 2.0f, bBoxSize, bBoxSize);
    }
}

void TransferFunctionEditorControlPoint::hoverEnterEvent(QGraphicsSceneHoverEvent* event) {
    prepareGeometryChange();
    size_ += 5.0f;
    showLabel_ = true;
    update();
}

void TransferFunctionEditorControlPoint::hoverLeaveEvent(QGraphicsSceneHoverEvent* event) {
    prepareGeometryChange();
    size_ -= 5.0f;
    showLabel_ = false;
    update();
}

QVariant TransferFunctionEditorControlPoint::itemChange(GraphicsItemChange change,
                                                        const QVariant& value) {
    if (change == QGraphicsItem::ItemPositionChange && scene()) {
        // constrain positions to valid view positions
        QPointF newPos = value.toPointF();
        QRectF rect = scene()->sceneRect();

        if (!rect.contains(newPos)) {
            newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
            newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
        }

        // update the associated transfer function data point
        QPointF controlPointPos = pos();
        if (!isEditingPoint_) {
            isEditingPoint_ = true;
            dataPoint_->setPosA(
                vec2(controlPointPos.x() / rect.width(), controlPointPos.y() / rect.height()),
                controlPointPos.y() / rect.height());
            isEditingPoint_ = false;
        }

        // return the constraint position
        return newPos;
    }

    return QGraphicsItem::itemChange(change, value);
}

void TransferFunctionEditorControlPoint::onTransferFunctionPointChange(
    const TransferFunctionDataPoint* p) {
    if (!isEditingPoint_) {
        isEditingPoint_ = true;
        QRectF rect = scene()->sceneRect();
        setPos(p->getPos().x * rect.width(), p->getPos().y * rect.height());
        isEditingPoint_ = false;
    }
}

QRectF TransferFunctionEditorControlPoint::calculateLabelRect() const {
    QRectF rect;
    if (dataPoint_->getPos().x > 0.5f) {
        rect.setX(-0.5 * size_ - textWidth_);
    } else {
        rect.setX(0.5 * size_);
    }
    if (dataPoint_->getPos().y > 0.5f) {
        rect.setY(0.5 * size_);
    } else {
        rect.setY(-0.5 * size_ - textHeight_);
    }
    rect.setHeight(textHeight_);
    rect.setWidth(textWidth_);
    return rect;
}

void TransferFunctionEditorControlPoint::setDataMap(const DataMapper& dataMap) {
    dataMap_ = dataMap;
}

inviwo::DataMapper TransferFunctionEditorControlPoint::getDataMap() const {
    return dataMap_;
}

const double TransferFunctionEditorControlPoint::textHeight_ = 20;
const double TransferFunctionEditorControlPoint::textWidth_ = 180;

}  // namespace