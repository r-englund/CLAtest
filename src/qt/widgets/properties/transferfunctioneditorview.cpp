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

namespace inviwo{
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

void TransferFunctionEditorView::setMaskMin( const int maskMin ){viewMaskMin_ = maskMin;}

void TransferFunctionEditorView::setMaskMax( const int maskMax ){viewMaskMax_ = maskMax;}

}