#ifndef IVW_LINKGRAPHICSITEM_H
#define IVW_LINKGRAPHICSITEM_H

#include <QGraphicsLineItem>
#include <QPainterPath>

#include "inviwo/core/ports/port.h"
#include "inviwo/qt/editor/processorgraphicsitem.h"

namespace inviwo {


class LinkGraphicsItem : public QGraphicsItem {

public:
    LinkGraphicsItem(QPointF startPoint, QPointF endPoint, bool layoutOption=true, ivec3 color=ivec3(38,38,38), bool dragMode=true);
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

protected:
    void setLayoutOption(bool layoutOption) {verticalLayout_=layoutOption;}
    bool getLayoutOption() {return verticalLayout_;}

private:
    QPointF startPoint_;
    QPointF endPoint_;
    bool verticalLayout_;
    QColor color_;
    bool dragMode_;

    virtual QPainterPath obtainCurvePath() const;
};


/**
 * Graphical representation of the connection between two ports in the network editor.
 */

class LinkConnectionGraphicsItem : public LinkGraphicsItem {

public:
    /**
     * Construct a new graphical representation between the outport of the outProcessor
     * and the inport of the inProcessor. Data is assumed to flow from outport to inport.
     * While the processors are provided through their graphical representations
     * (@see ProcessorGraphicsItem), the ports directly specified as used in the data flow
     * network.
     *
     * @param outProcessor Graphical representation of the source processor.
     * @param outport Source port.
     * @param inProcessor Graphical representation of the destination processor.
     * @param inport Destination port.
     */
    LinkConnectionGraphicsItem(ProcessorGraphicsItem* outProcessor, ProcessorGraphicsItem* inProcessor,
                           bool layoutOption=true);
    ~LinkConnectionGraphicsItem();

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

    void flipLayout();

private:
    ProcessorGraphicsItem* outProcessor_; ///< Processor representation from which the connection starts
    ProcessorGraphicsItem* inProcessor_; ///< Processor representation to which the connection goes to 
};

} // namespace

#endif // IVW_CONNECTIONGRAPHICSITEM_H