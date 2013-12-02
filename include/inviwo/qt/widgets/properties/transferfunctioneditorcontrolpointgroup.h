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

#ifndef IVW_TRANSFERFUNCTIONEDITORCONTROLPOINTGROUP_H
#define IVW_TRANSFERFUNCTIONEDITORCONTROLPOINTGROUP_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <QGraphicsView>
#include <QPainter>
#include <QPainterPath>

#include <inviwo/core/ports/port.h>
#include <inviwo/qt/editor/processorgraphicsitem.h>
#include <inviwo/qt/widgets/properties/transferfunctioneditorcontrolpoint.h>

namespace inviwo {
    class IVW_QTWIDGETS_API TransferFunctionEditorControlPointGroup : public QObject, public QGraphicsItem{
            Q_OBJECT
    public:
        TransferFunctionEditorControlPointGroup();
        ~TransferFunctionEditorControlPointGroup();

        //override for qgraphicsitem_cast (refer qt documentation)
        enum { Type = UserType + ConnectionGraphicsType };
        int type() const  {return Type; }

        /**Paint method
        * Overloaded paint method from QGraphicsItem. Here the actual representation is drawn.
        */
        void paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget);
        QRectF boundingRect() const;


        void addPoint(TransferFunctionEditorControlPoint*);

		void notify();

    protected:
        void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
        void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
	
    private:
        float size_; ///< size for drawing the points
        TransferFunctionDataPoint* datapoint_; ///<The TransferFunctionDataPoint the controlpoint gets all its data from
		float viewWidth_;
		float viewHeight_;
    };
}// namespace
#endif // IVW_TRANSFERFUNCTIONEDITORCONTROLPOINTGROUP_H