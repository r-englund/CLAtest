/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Main file authors: Viktor Axelsson, Timo Ropinski
 *
 *********************************************************************************/

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
#include <inviwo/core/util/factory.h>

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/properties/transferfunctioneditorcontrolpoint.h>

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

    float getZoomRangeXMin() const { return zoomRangeXMin_; }
    void setZoomRangeXMin(float min) { zoomRangeXMin_ = min; }

    float getZoomRangeXMax() const { return zoomRangeXMax_; }
    void setZoomRangeXMax(float max) { zoomRangeXMax_ = max; }

    float getZoomRangeYMin() const { return zoomRangeYMin_; }
    void setZoomRangeYMin(float min) { zoomRangeYMin_ = min; }

    float getZoomRangeYMax() const { return zoomRangeYMax_; }
    void setZoomRangeYMax(float max) { zoomRangeYMax_ = max; }

    QGraphicsView* getView() { return view_; }
    void redrawConnections();

    void recalculateControlPoints();

signals:
    void doubleClick();

public slots:
    void resetTransferFunction();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* e);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* e);
    void keyPressEvent(QKeyEvent* e);

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

    float zoomRangeXMin_;
    float zoomRangeXMax_;
    float zoomRangeYMin_;
    float zoomRangeYMax_;

    QGraphicsView* view_;
    TransferFunction* transferFunction_; ///< Pointer to widget's member variable

    QGraphicsPathItem* graphicsPathItem_;
    std::vector<TransferFunctionEditorControlPoint*> controlPoints_; ///< Control points in the transfer function graph

    bool mouseDrag_;
};

} // namespace

#endif // IVW_TRANSFERFUNCTIONEDITOR_H