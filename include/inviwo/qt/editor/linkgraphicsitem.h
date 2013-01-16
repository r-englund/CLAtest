#ifndef IVW_LINKGRAPHICSITEM_H
#define IVW_LINKGRAPHICSITEM_H

#include <QGraphicsLineItem>
#include <QPainterPath>

#include "inviwo/core/ports/port.h"
#include "inviwo/qt/editor/processorgraphicsitem.h"

namespace inviwo {


class LinkGraphicsItem : public QGraphicsItem {

public:
    LinkGraphicsItem(QPointF startPoint, QPointF endPoint, ivec3 color=ivec3(38,38,38), bool dragMode=true);
    ~LinkGraphicsItem();

    virtual QPainterPath shape() const;

    void setStartPoint(QPointF startPoint) { startPoint_ = startPoint; }
    QPointF getStartPoint() const { return startPoint_; }

    void setEndPoint(QPointF endPoint) { endPoint_ = endPoint; }
    QPointF getEndPoint() const { return endPoint_; }

    /**
     * Overloaded paint method from QGraphicsItem. Here the actual representation is drawn.
     */
    void paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget);
    QRectF boundingRect() const;

    //override for qgraphicsitem_cast (refer qt documentation)
    enum { Type = UserType + LinkGraphicsType };
    int type() const  {return Type; }

private:
    QPointF startPoint_;
    QPointF endPoint_;
    QColor color_;
    bool dragMode_;

    virtual QPainterPath obtainCurvePath() const;
};


class LinkConnectionGraphicsItem : public LinkGraphicsItem {

public:
    
    LinkConnectionGraphicsItem(ProcessorGraphicsItem* outProcessor, ProcessorGraphicsItem* inProcessor);
    ~LinkConnectionGraphicsItem();
    
    ProcessorGraphicsItem* getOutProcessor() const { return outProcessor_; }    
    ProcessorGraphicsItem* getInProcessor() const { return inProcessor_; }

private:
    ProcessorGraphicsItem* outProcessor_; ///< Processor representation from which the connection starts
    ProcessorGraphicsItem* inProcessor_; ///< Processor representation to which the connection goes to 
};

} // namespace

#endif // IVW_CONNECTIONGRAPHICSITEM_H