/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#ifndef IVW_LINKGRAPHICSITEM_H
#define IVW_LINKGRAPHICSITEM_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <QGraphicsLineItem>
#include <QPainterPath>

#include <inviwo/core/ports/port.h>
#include <inviwo/qt/editor/processorgraphicsitem.h>

namespace inviwo {


class IVW_QTEDITOR_API LinkGraphicsItem : public QGraphicsItem {

public:
    LinkGraphicsItem(QPointF startPoint, QPointF endPoint, ivec3 color=ivec3(38,38,38));
    ~LinkGraphicsItem();

    virtual QPainterPath shape() const;

    void setStartPoint(QPointF startPoint) { prepareGeometryChange(); startPoint_ = startPoint; }
    QPointF getStartPoint() const { return startPoint_; }

    void setEndPoint(QPointF endPoint) { prepareGeometryChange(); endPoint_ = endPoint; }
    QPointF getEndPoint() const { return endPoint_; }

    /**
     * Overloaded paint method from QGraphicsItem. Here the actual representation is drawn.
     */
    void paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget);
    virtual QRectF boundingRect() const;

    //override for qgraphicsitem_cast (refer qt documentation)
    enum { Type = UserType + LinkGraphicsType };
    int type() const  {return Type; }

private:
    QPointF startPoint_;
    QPointF endPoint_;
    QColor color_;

    virtual QPainterPath obtainCurvePath() const;
};


class IVW_QTEDITOR_API LinkConnectionGraphicsItem : public LinkGraphicsItem {

public:
    
    LinkConnectionGraphicsItem(ProcessorGraphicsItem* outProcessor, ProcessorGraphicsItem* inProcessor);
    ~LinkConnectionGraphicsItem();
    
    ProcessorGraphicsItem* getSrcProcessorGraphicsItem() const { return outProcessor_; }    
    ProcessorGraphicsItem* getDestProcessorGraphicsItem() const { return inProcessor_; }
    virtual QRectF boundingRect() const;

private:
    virtual QPainterPath obtainCurvePath() const;
    
    ProcessorGraphicsItem* outProcessor_; ///< Processor representation from which the connection starts
    ProcessorGraphicsItem* inProcessor_; ///< Processor representation to which the connection goes to 
};

} // namespace

#endif // IVW_CONNECTIONGRAPHICSITEM_H