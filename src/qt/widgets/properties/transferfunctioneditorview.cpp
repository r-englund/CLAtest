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

#include <inviwo/qt/widgets/properties/transferfunctioneditorview.h>
#include <inviwo/qt/widgets/properties/transferfunctionpropertydialog.h>
#include <QVarLengthArray>

namespace inviwo {

TransferFunctionEditorView::TransferFunctionEditorView(TransferFunctionProperty* tfProperty)
    : tfProperty_(tfProperty)
{
    setMouseTracking(true);
    setRenderHint(QPainter::Antialiasing, true);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

void TransferFunctionEditorView::resizeEvent(QResizeEvent* event) {
    fitInView(scene()->sceneRect(), Qt::IgnoreAspectRatio);
	QGraphicsView::resizeEvent(event);
}

void TransferFunctionEditorView::drawForeground(QPainter* painter, const QRectF &rect) {
    if (mask_.x > 0.0f) {
        int leftMaskBorder = mask_.x*sceneRect().width();
        painter->fillRect(0, 0, leftMaskBorder, rect.height(), QColor(25,25,25,100));
        painter->drawLine(leftMaskBorder, 0, leftMaskBorder, rect.height());
    }
    if (mask_.y < 1.0f) {
        int rightMaskBorder = mask_.y*sceneRect().width();
        // add 10 to width to compensate scaling differences between scene and view
        painter->fillRect(rightMaskBorder, 0, sceneRect().width()-rightMaskBorder+10, rect.height(), QColor(25,25,25,100));
        painter->drawLine(rightMaskBorder, 0, rightMaskBorder, rect.height());
    }
	QGraphicsView::drawForeground(painter, rect);
}

void TransferFunctionEditorView::drawBackground(QPainter* painter, const QRectF& rect) {
    painter->fillRect(rect, QColor(119,136,221));
    painter->drawRect(rect);

    // obtain histogram
    std::vector<float> histogram = tfProperty_->getHistogram();

    // draw histogram
    QVarLengthArray<QLineF, 100> linesY;
    QRectF sRect = sceneRect();
    for (size_t i=0; i<histogram.size(); i++)
        linesY.append(QLineF(((float)i/(float)histogram.size())*sRect.width(), 0.0,
                             ((float)i/(float)histogram.size())*sRect.width(), histogram[i]*sRect.height()));
    qreal lineWidth = sRect.width()/histogram.size();
    painter->setPen(QPen(QColor(68,102,170), lineWidth));
    painter->drawLines(linesY.data(), linesY.size());
}

void TransferFunctionEditorView::updateZoom() {
    QRectF zoomRect;
    zoomRect.setLeft(zoomH_.x*scene()->sceneRect().width());
    zoomRect.setRight(zoomH_.y*scene()->sceneRect().width());
    zoomRect.setBottom(zoomV_.x*scene()->sceneRect().height());
    zoomRect.setTop(zoomV_.y*scene()->sceneRect().height());
    fitInView(zoomRect, Qt::IgnoreAspectRatio);
}

void TransferFunctionEditorView::zoomHorizontally(int zoomHMin, int zoomHMax) {
    // normalize zoom values, as sliders in TransferFunctionPropertyDialog
    // have the range [0...100]
    zoomH_.x = zoomHMin / 100.0f;
    zoomH_.y = zoomHMax / 100.0f;
    // avoid degenerated zoom area
    if (abs(zoomH_.x-zoomH_.y)<0.05f)
        if (zoomH_.y == 1.0f) zoomH_.x=zoomH_.y-0.05f;
        else zoomH_.y=zoomH_.x+0.05f;
    updateZoom();
}

void TransferFunctionEditorView::zoomVertically(int zoomVMin, int zoomVMax) {
    // normalize zoom values, as sliders in TransferFunctionPropertyDialog
    // have the range [0...100]
    // and flip/rescale values to compensate slider layout
    zoomV_.x = (100.0-zoomVMax) / 100.0f;
    zoomV_.y = (100.0-zoomVMin) / 100.0f;
    // avoid degenerated zoom area
    if (abs(zoomV_.x-zoomV_.y)<0.05f)
        if (zoomV_.y == 1.0f) zoomV_.x=zoomV_.y-0.05f;
        else zoomV_.y=zoomV_.x+0.05f;
    updateZoom();
}

} // namespace inviwo