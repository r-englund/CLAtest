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

/** \Inherited from QGraphicsScene for editing a transfer function (TransferFunction)
*
*  Class for graphical configuration of a transfer function
*  
*/

#ifndef IVW_TRANSFERFUNCTIONEDITOR_H
#define IVW_TRANSFERFUNCTIONEDITOR_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/transferfunction.h>
#include <inviwo/core/io/serialization/ivwserializer.h>
#include <inviwo/core/io/serialization/ivwdeserializer.h>
#include <inviwo/core/network/processornetworkevaluator.h>
#include <inviwo/core/processors/processorfactory.h>
#include <inviwo/core/util/inviwofactorybase.h>

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/properties/transferfunctioneditorcontrolpoint.h>
#include <inviwo/qt/widgets/properties/transferfunctioneditorcontrolpointgroup.h>
#include <inviwo/qt/widgets/properties/transferfunctioneditorlineitem.h>

#include <inviwo/qt/widgets/propertylistwidget.h>

#include <QBrush>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QLineF>
#include <QPainter>
#include <QPen>
#include <QPointF>
#include <vector>

namespace inviwo {

class TransferFunctionEditor : public QGraphicsScene {

    Q_OBJECT

public :
    /** \TransferFunctionEditor constructor
    *         
    *  Main constructor, creates control points
    */
    TransferFunctionEditor(TransferFunction* transferFunction, QGraphicsView* view);
    ~TransferFunctionEditor();

    void updateControlPointView();

    const float getZoomRangeXMin() { return zoomRangeXMin_; }
    void setZoomRangeXMin(float min) { zoomRangeXMin_ = min; }

    const float getZoomRangeXMax() { return zoomRangeXMax_; }
    void setZoomRangeXMax(float max) { zoomRangeXMax_ = max; }

    const float getZoomRangeYMin() { return zoomRangeYMin_; }
    void setZoomRangeYMin(float min) { zoomRangeYMin_ = min; }

    const float getZoomRangeYMax() { return zoomRangeYMax_; }
    void setZoomRangeYMax(float max) { zoomRangeYMax_ = max; }

    QGraphicsView* getView() { return view_; }

signals:
    void doubleClick();
    void controlPointsChanged();

public slots:
    void resetTransferFunction();

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent* e);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* e);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* e);
	void keyPressEvent(QKeyEvent *e);

    /** \Add new control point
    *      Adds a new control point at the event position
    *
    *      Adds a new control point the the points_ array, adds a new line item to the lines_ array,
    *      sorts the points_ array and updates the line items to go to and from the correct points.
    *      Runs CalcTransferValues to update the TransferFunction data Image
	*/
    void addControlPoint(QPointF pos);
    void addControlPoint(QPointF pos, vec4 color);

	void removeControlPoint(TransferFunctionEditorControlPoint* p);

    TransferFunctionEditorControlPoint* getControlPointGraphicsItemAt(const QPointF pos) const;

private :
    void addControlPoint(QPointF pos, TransferFunctionDataPoint* dataPoint);
    void updateLines();

    float zoomRangeXMin_;
    float zoomRangeXMax_;
    float zoomRangeYMin_;
    float zoomRangeYMax_;

	QGraphicsView* view_;
	TransferFunction* transferFunction_; ///< Pointer to widget's member variable

    std::vector<TransferFunctionEditorLineItem*> lines_; ///< Vector for the lines between the control points
	std::vector<TransferFunctionEditorControlPoint*> controlPoints_; ///< Control points in the transfer function graph
	std::vector<TransferFunctionEditorControlPointGroup*> controlPointGroups_; ///< Control points in the transfer function graph
};

} // namespace

#endif // IVW_TRANSFERFUNCTIONEDITOR_H