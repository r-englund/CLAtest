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

namespace inviwo{

TransferFunctionEditorView::TransferFunctionEditorView(TransferFunctionProperty* tfProperty)
    : tfProperty_(tfProperty)
{
}

void TransferFunctionEditorView::resizeEvent(QResizeEvent* event) {
	emit resized();
	static_cast<TransferFunctionEditor*>(scene())->repositionPoints();
	QGraphicsView::resizeEvent(event);
}

void TransferFunctionEditorView::drawForeground(QPainter* painter, const QRectF &rect) {
    if (mask_.x > zoomH_.x) {
        int leftMaskBorder = ((mask_.x-zoomH_.x)/(zoomH_.y-zoomH_.x)*rect.width());
        painter->fillRect(0, 0, leftMaskBorder, rect.height(), QColor(25,25,25,100));
        painter->drawLine(leftMaskBorder, 0, leftMaskBorder, rect.height());
    }
    if (mask_.y < zoomH_.y) {
        int rightMaskBorder = ((mask_.y-zoomH_.x)/(zoomH_.y-zoomH_.x)*rect.width());
        painter->fillRect(rightMaskBorder, 0, rect.width()-rightMaskBorder, rect.height(), QColor(25,25,25,100));
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
    unsigned int firstLine = zoomH_.x*histogram.size();
    unsigned int lastLine  = zoomH_.y*histogram.size();
    unsigned int numLines = lastLine-firstLine;
    for (unsigned int i=firstLine; i<lastLine; i++)
        linesY.append(QLineF((i-firstLine)*(rect.width()/numLines), 0.0,
                             (i-firstLine)*(rect.width()/numLines), histogram[i]*rect.height()));
    qreal lineWidth = rect.width()/numLines;
    painter->setPen(QPen(QColor(68,102,170), lineWidth));
    painter->drawLines(linesY.data(), linesY.size());
}

}