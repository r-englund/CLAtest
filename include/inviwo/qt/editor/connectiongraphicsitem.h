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
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#ifndef IVW_CONNECTIONGRAPHICSITEM_H
#define IVW_CONNECTIONGRAPHICSITEM_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <QGraphicsLineItem>
#include <QPainterPath>

#include <inviwo/core/ports/inport.h>
#include <inviwo/core/ports/outport.h>
#include <inviwo/qt/editor/processorgraphicsitem.h>

namespace inviwo {


class IVW_QTEDITOR_API CurveGraphicsItem : public QGraphicsItem {

public:
    CurveGraphicsItem(QPointF startPoint, QPointF endPoint, uvec3 color=uvec3(38,38,38));
    ~CurveGraphicsItem();

    virtual QPainterPath shape() const;

    void setStartPoint(QPointF startPoint) { prepareGeometryChange(); startPoint_ = startPoint; midPoint_ = startPoint; }
    QPointF getStartPoint() const { return startPoint_; }
 
    void setEndPoint(QPointF endPoint) { prepareGeometryChange(); endPoint_ = endPoint; }
    QPointF getEndPoint() const { return endPoint_; }

	void setMidPoint(QPointF midPoint) { prepareGeometryChange(); midPoint_ = midPoint; }
	void clearMidPoint() { prepareGeometryChange(); midPoint_ = startPoint_; }
	QPointF getMidPoint() const { return midPoint_; }

	void setColor(QColor color) { color_ = color; }
	QColor getColor() { return color_; }

    void resetBorderColors();
    void setBorderColor(QColor borderColor) { borderColor_ = borderColor; }
    void setSelectedBorderColor(QColor selectedBorderColor) { selectedBorderColor_ = selectedBorderColor; }
	void setBorderColors(QColor hoverInputColor, QColor hoverOutputColor) { hoverInputColor_ = hoverInputColor; hoverOutputColor_ = hoverOutputColor; }

    //override for qgraphicsitem_cast (refer qt documentation)
    enum { Type = UserType + ConnectionGraphicsType };
    int type() const  {return Type; }

    /**
     * Overloaded paint method from QGraphicsItem. Here the actual representation is drawn.
     */
    virtual void paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget);
    QRectF boundingRect() const;

	virtual QPainterPath obtainCurvePath() const;
	QPainterPath obtainCurvePath(QPointF startPoint, QPointF endPoint);

protected:
    
    QPointF startPoint_;
    QPointF endPoint_;
	QPointF midPoint_;

    QColor color_;
    QColor borderColor_;
    QColor selectedBorderColor_;
    QColor hoverInputColor_;
    QColor hoverOutputColor_;
};


/**
 * Graphical representation of the connection between two ports in the network editor.
 */

class IVW_QTEDITOR_API ConnectionGraphicsItem : public CurveGraphicsItem {

public:
    /**
     * Construct a new graphical representation between the outport of the outProcessor
     * and the inport of the inProcessor. Data is assumed to flow from outport to inport.
     * While the processors are provided through their graphical representations
     * (@see ProcessorGraphicsItem), the ports are directly specified as used in the data
     * flow network.
     *
     * @param outProcessor Graphical representation of the source processor.
     * @param outport Source port.
     * @param inProcessor Graphical representation of the destination processor.
     * @param inport Destination port.
     */
    ConnectionGraphicsItem(ProcessorGraphicsItem* outProcessor, Outport* outport,
                           ProcessorGraphicsItem* inProcessor, Inport* inport);
    ~ConnectionGraphicsItem();

    /**
     * Return the graphical representation of the processor from which the connection starts.
     * This processors provides the data within the data flow network.
     */
    ProcessorGraphicsItem* getOutProcessor() const { return outProcessor_; }
    /**
     * Return the graphical representation of the processor to which the connection goes to.
     * This processors receives the data within the data flow network.
     */
    ProcessorGraphicsItem* getInProcessor() const { return inProcessor_; }

    /**
     * Return the outport from which the connection starts.
     */
    Outport* getOutport() const { return outport_; }
    /**
     * Return the inport to which the connection goes to.
     */
    Inport* getInport() const { return inport_; }

private:
    ProcessorGraphicsItem* outProcessor_; ///< Processor representation from which the connection starts
    ProcessorGraphicsItem* inProcessor_; ///< Processor representation to which the connection goes to
    Outport* outport_; ///< Port of outProcessor_ from which the connection starts
    Inport* inport_; ///< Port of inProcessor_ to which the connection goes to
};

} // namespace

#endif // IVW_CONNECTIONGRAPHICSITEM_H