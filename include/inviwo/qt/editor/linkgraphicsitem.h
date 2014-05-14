/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
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
 * Main file author: Sathish Kottravel
 *
 *********************************************************************************/

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
    LinkGraphicsItem(QPointF startPoint, QPointF endPoint, ivec3 color = ivec3(255, 255, 255));
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