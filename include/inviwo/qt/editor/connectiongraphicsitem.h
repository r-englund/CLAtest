#ifndef IVW_CONNECTIONGRAPHICSITEM_H
#define IVW_CONNECTIONGRAPHICSITEM_H

#include <QGraphicsLineItem>

#include "inviwo/core/ports/port.h"
#include "inviwo/qt/editor/processorgraphicsitem.h"

namespace inviwo {

/**
 * Graphical representation of the connection between two ports in the network editor.
 */

class ConnectionGraphicsItem : public QGraphicsLineItem {

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
    ConnectionGraphicsItem(ProcessorGraphicsItem* outProcessor, Port* outport,
                           ProcessorGraphicsItem* inProcessor, Port* inport);
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
    Port* getOutport() const { return outport_; }
    /**
     * Return the inport to which the connection goes.
     */
    Port* getInport() const { return inport_; }

protected:
    /**
     * Overloaded paint method from QWidget. Here the actual representation is drawn.
     */
    void paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget);

private:
    ProcessorGraphicsItem* outProcessor_; ///< Processor representation from which the connection starts
    ProcessorGraphicsItem* inProcessor_; ///< Processor representation to which the connection goes to
    Port* outport_; ///< Port of outProcessor_ from which the connection starts
    Port* inport_; ///< Port of inProcessor_ to which the connection goes to
};

} // namespace

#endif // IVW_CONNECTIONGRAPHICSITEM_H