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

const int TransferFunctionEditorView::GRID_SPACING = 25;

void TransferFunctionEditorView::resizeEvent ( QResizeEvent * event ){
	emit resized();
	static_cast<TransferFunctionEditor*>(scene())->repositionPoints();
	QGraphicsView::resizeEvent( event );
}

void TransferFunctionEditorView::drawForeground( QPainter *painter, const QRectF &rect ){
	painter->save();
	painter->fillRect(QRectF(0, 0, viewMaskMin_, this->height()), QBrush(QColor(64, 64, 64, 192)));
	painter->fillRect(QRectF(viewMaskMax_,0, this->width(), this->height()), QBrush(QColor(64, 64, 64, 192)));	

	painter->setWorldMatrixEnabled(true);
	painter->restore();
	QGraphicsView::drawForeground(painter, rect);
}

void TransferFunctionEditorView::drawBackground(QPainter* painter, const QRectF & rect) {
    painter->setWorldMatrixEnabled(true);
    painter->fillRect(rect, Qt::darkGray);

    qreal left = int(rect.left()) - (int(rect.left()) % GRID_SPACING );
    qreal top = int(rect.top()) - (int(rect.top()) % GRID_SPACING );

    QVarLengthArray<QLineF, 100> linesX;
    for (qreal x = left; x < rect.right(); x += GRID_SPACING )
        linesX.append(QLineF(x, rect.top(), x, rect.bottom()));

    QVarLengthArray<QLineF, 100> linesY;
    for (qreal y = top; y < rect.bottom(); y += GRID_SPACING )
        linesY.append(QLineF(rect.left(), y, rect.right(), y));

    painter->drawLines(linesX.data(), linesX.size());
    painter->drawLines(linesY.data(), linesY.size());
}

void TransferFunctionEditorView::setMaskMin( const int maskMin ){viewMaskMin_ = maskMin;}

void TransferFunctionEditorView::setMaskMax( const int maskMax ){viewMaskMax_ = maskMax;}

}