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

#ifndef IVW_TRANSFERFUNCTIONEDITORVIEW_H
#define IVW_TRANSFERFUNCTIONEDITORVIEW_H

#include <inviwo/core/properties/transferfunctionproperty.h>

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/properties/transferfunctioneditor.h>
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>

#include <QtEvents>
#include <QGraphicsScene>
#include <QGraphicsView>

namespace inviwo {
class IVW_QTWIDGETS_API TransferFunctionEditorView : public QGraphicsView {

	Q_OBJECT

public:
    TransferFunctionEditorView(TransferFunctionProperty* tfProperty);

    void setMask(float maskMin, float maskMax) { if (maskMax<maskMin) maskMax=maskMin; mask_ = vec2(maskMin, maskMax); }

signals:
    void resized();

public slots:
    void zoomHorizontally(int zoomHMin, int zoomHMax);
    void zoomVertically(int zoomVMin, int zoomVMax);

protected:
    void updateZoom();
	void resizeEvent(QResizeEvent * event);

	void drawForeground(QPainter *painter, const QRectF &rect);
    void drawBackground(QPainter* painter, const QRectF& rect);

private:
    TransferFunctionProperty* tfProperty_;
    vec2 mask_;
    vec2 zoomH_;
    vec2 zoomV_;
};
} // namespace

#endif // IVW_TRANSFERFUNCTIONEDITORVIEW_H