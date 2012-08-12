#ifndef IVW_CONNECTIONGRAPHICSITEM_H
#define IVW_CONNECTIONGRAPHICSITEM_H

#include <QGraphicsLineItem>

#include "inviwo/core/ports/port.h"



namespace inviwo {

class ProcessorGraphicsItem;

class ConnectionGraphicsItem : public QGraphicsLineItem {
    //Q_OBJECT

public:
    ConnectionGraphicsItem(ProcessorGraphicsItem* outProcessor, Port* outport, ProcessorGraphicsItem* inProcessors, Port* inport);
    ~ConnectionGraphicsItem();

    ProcessorGraphicsItem* getOutProcessor() { return outProcessor_; }
    ProcessorGraphicsItem* getInProcessor() { return inProcessor_; }

    Port* getOutport() { return outport_; }
    Port* getInport() { return inport_; }

protected:
    bool event(QEvent* event);

    void paint(QPainter* p, const QStyleOptionGraphicsItem* opt, QWidget* w);
    //QVariant itemChange(GraphicsItemChange change, const QVariant & value);

private:
    friend class NetworkEditor;
    friend class ProcessorGraphicsItem;

    ProcessorGraphicsItem* outProcessor_;
    ProcessorGraphicsItem* inProcessor_;
    Port* outport_;
    Port* inport_;

};

} // namespace

#endif // IVW_CONNECTIONGRAPHICSITEM_H