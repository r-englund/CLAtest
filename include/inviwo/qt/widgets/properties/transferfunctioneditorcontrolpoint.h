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

/** \TransferFunctionEditorControlPoint class
 *         A point in the TransferFunctionEditor
 *
 *         Overloaded QGraphicsItem used in TransferFunctionEditor. 
 *         Position of control points are used to
 *         calulate the TransferFunction Image values, currently has one parameter
 */

#ifndef IVW_TRANSFERFUNCTIONEDITORCONTROLPOINT_H
#define IVW_TRANSFERFUNCTIONEDITORCONTROLPOINT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/core/datastructures/transferfunctiondatapoint.h>
#include <inviwo/core/datastructures/datamapper.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <QGraphicsItem>


#include <QPainterPath>

class QGraphicsSceneHoverEvent;
class QPainter;

namespace inviwo {

class TransferFunctionControlPointConnection;

class IVW_QTWIDGETS_API TransferFunctionEditorControlPoint : public QGraphicsItem,
                                                             public TransferFunctionPointObserver {
public:
    TransferFunctionEditorControlPoint(TransferFunctionDataPoint* dataPoint,
                                       const DataMapper& dataMap);
    ~TransferFunctionEditorControlPoint();

    void setDataPoint(TransferFunctionDataPoint* dataPoint);
    TransferFunctionDataPoint* getPoint() const;

    void setDataMap(const DataMapper& dataMap);
    DataMapper getDataMap() const;

    // override for qgraphicsitem_cast (refer qt documentation)
    enum { Type = UserType + TransferFunctionEditorControlPointType };
    int type() const { return Type; }

    virtual void onTransferFunctionPointChange(const TransferFunctionDataPoint* p);
    bool operator==(const TransferFunctionDataPoint* point) const;

    TransferFunctionControlPointConnection* left_;
    TransferFunctionControlPointConnection* right_;

protected:
    // Overload
    void paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget);
    QRectF boundingRect() const;
    QVariant itemChange(GraphicsItemChange change, const QVariant& value);

    QPainterPath shape() const;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);

private:
    QRectF calculateLabelRect() const;

    static const double textWidth_;
    static const double textHeight_;
    float size_;  ///< size for drawing the points
    bool showLabel_;
    bool isEditingPoint_;
    TransferFunctionDataPoint*
        dataPoint_;  ///<The TransferFunctionDataPoint the control point gets all its data from
    DataMapper dataMap_;
};

}  // namespace
#endif  // IVW_TRANSFERFUNCTIONEDITORCONTROLPOINT_H