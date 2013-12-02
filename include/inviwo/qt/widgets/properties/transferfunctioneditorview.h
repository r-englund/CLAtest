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

//Inviwo includes
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/properties/transferfunctioneditor.h>
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>

//Qt includes
#include <QtEvents>
#include <QGraphicsScene>
#include <QGraphicsView>

namespace inviwo {
class IVW_QTWIDGETS_API TransferFunctionEditorView : public QGraphicsView {

	Q_OBJECT

public:    
    void setMaskMin( const int maskMin );
    void setMaskMax( const int maskMax );

signals:
	void resized();

protected:
	void resizeEvent ( QResizeEvent * event );
	void drawForeground(QPainter *painter, const QRectF &rect);

private:
    float viewMaskMin_;
    float viewMaskMax_;
};
} // namespace

#endif // IVW_TRANSFERFUNCTIONEDITORVIEW_H