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
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QPainterPath>

#include <inviwo/core/ports/port.h>
#include <inviwo/qt/widgets/properties/transferfunctioneditorcontrolpoint.h>

namespace inviwo {

class IVW_QTWIDGETS_API TransferFunctionEditorLineItem : public QGraphicsItem {

public:
	TransferFunctionEditorLineItem();
    /** 
     * \brief TransferFunctionEditorLineItem Constructor
     *
     * Constructor for TransferFunctionEditorLineItem
     * 
     * @param TransferFunctionDataPoint * Pointer to the point where the line starts
     * @param TransferFunctionDataPoint * Pointer to the point where the line ends
	 */
	TransferFunctionEditorLineItem(TransferFunctionEditorControlPoint* start, TransferFunctionEditorControlPoint* finish);


	TransferFunctionEditorLineItem(TransferFunctionEditorControlPoint* start, int dir);
    /** 
     * \brief TransferFunctionEditorLineItem Constructor
     *
     * TransferFunctionEditorLineItem default constructor
     */
    //TransferFunctionEditorLineItem();
	~TransferFunctionEditorLineItem();

	TransferFunctionEditorControlPoint* getStart();
	TransferFunctionEditorControlPoint* getFinish();
	/** 
	 * \brief Sets the lines starting point
	 *
	 * Changes the point where the line starts
	 * 
	 * @param TransferFunctionDataPoint * start the new start-point
	 */
	void setStart(TransferFunctionEditorControlPoint* start);

	/** 
	 * \brief Sets the lines ending point
	 *
	 * Changes the point where the line ends
	 * 
	 * @param TransferFunctionDataPoint * finish the new end-point
	 */
	void setFinish(TransferFunctionEditorControlPoint* finish);

	void setDirection(int dir);
	QPainterPath shape() const;
	void initiateLineItem();

    //override for qgraphicsitem_cast (refer qt documentation)
    enum { Type = UserType + 3};
    int type() const  {return Type; }

    virtual void paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget);
    QRectF boundingRect() const;

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent);
    void mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent);

private:
    TransferFunctionEditorControlPoint* start_;
    TransferFunctionEditorControlPoint* finish_;
	QPointF mouseDownPos_;
	int direction_;
};

}// namespace
#endif // IVW_TRANSFERFUNCTIONEDITORLINEITEM_H