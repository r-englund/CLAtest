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

/** \TransferFunctionEditorLineItem class
 *         A line between control points in the TransferFunctionEditor
 *
 *         A QGraphicsItem that draws a line between the start_ and finish_ TransferFunctionEditorControlPoints
 *         
 */

#ifndef IVW_TRANSFERFUNCTIONEDITORLINEITEM_H
#define IVW_TRANSFERFUNCTIONEDITORLINEITEM_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QGraphicsItem>
#include <QPainterPath>

namespace inviwo {

class IVW_QTWIDGETS_API TransferFunctionEditorLineItem : public QGraphicsItem {

public:
    /** 
     * \brief TransferFunctionEditorLineItem Constructor
     *
     * Constructor for TransferFunctionEditorLineItem
     */
	TransferFunctionEditorLineItem(QPointF startPos, QPointF endPos);
    ~TransferFunctionEditorLineItem();

	/** 
	 * \brief Sets the lines starting point
	 *
	 * Changes the point where the line starts
	 */
    void setStartPos(QPointF startPos) { startPos_ = startPos; }
    QPointF getStartPos() { return startPos_; }

	/** 
	 * \brief Sets the lines ending point
	 *
	 * Changes the point where the line ends
	 */
    void setEndPos(QPointF endPos) { endPos_ = endPos; }
    QPointF getEndPos() { return endPos_; }

    virtual void paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget);
    QPainterPath shape() const;
    QRectF boundingRect() const;

    //override for qgraphicsitem_cast (refer qt documentation)
    enum { Type = UserType + 3};
    int type() const  {return Type; }

private:
    QPointF startPos_;
    QPointF endPos_;
};

}// namespace
#endif // IVW_TRANSFERFUNCTIONEDITORLINEITEM_H