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

/** \TransferFunctionEditorControlPoint class
 *         A point in the TransferFunctionEditor
 *
 *         Overloaded QGraphicsItem used in TransferFunctionEditor. Position of control points are used to
 *         calulate the TransferFunction Image values, currently has one parameter
 */
#ifndef IVW_TRANSFERFUNCTIONEDITORCONTROLPOINT_H
#define IVW_TRANSFERFUNCTIONEDITORCONTROLPOINT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
//#include <inviwo/qt/widgets/properties/transferfunctioneditorview.h>
#include <QGraphicsSceneEvent>
#include <QGraphicsView>
#include <QPainter>
#include <QPainterPath>

#include <inviwo/core/ports/port.h>

namespace inviwo {

class TransferFunctionDataPoint;

class IVW_QTWIDGETS_API TransferFunctionEditorControlPoint : public QObject, public QGraphicsItem {
        Q_OBJECT
public:
    /** \TransferFunctionEditorControlPoint constructor  
    *      Creates a TransferFunctionEditorControlPoint at a QPointF
    */        
    TransferFunctionEditorControlPoint(TransferFunctionDataPoint* datapoint);
    
    /** \TransferFunctionEditorControlPoint constructor  
    *      Default constructor
    */
    TransferFunctionEditorControlPoint();

    /** \TransferFunctionEditorControlPoint destructor  
    */
    ~TransferFunctionEditorControlPoint();

    //override for qgraphicsitem_cast (refer qt documentation)
    enum { Type = UserType + 2 };
    int type() const  {return Type; }

    /**Paint method
    * Overloaded paint method from QGraphicsItem. Here the actual representation is drawn.
    */
    virtual void paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget);
    QRectF boundingRect() const;

    TransferFunctionDataPoint* getPoint() const;
	TransferFunctionEditorControlPoint* getLeftNeighbour() const;
	TransferFunctionEditorControlPoint* getRightNeighbour() const;

	void setLeftNeighbour(TransferFunctionEditorControlPoint*);
	void setRightNeighbour(TransferFunctionEditorControlPoint*);

	QVariant itemChange(GraphicsItemChange change, const QVariant &value);
	void notify();

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:
    float size_; ///< size for drawing the points
    TransferFunctionDataPoint* datapoint_; ///<The TransferFunctionDataPoint the controlpoint gets all its data from
	TransferFunctionEditorControlPoint* leftNeighbour_;
	TransferFunctionEditorControlPoint* rightNeighbour_;
	float viewWidth_;
	float viewHeight_;
};

}// namespace
#endif // IVW_TRANSFERFUNCTIONEDITORCONTROLPOINT_H