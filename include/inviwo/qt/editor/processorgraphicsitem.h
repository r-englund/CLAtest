#ifndef IVW_PROCESSORGRAPHICSITEM_H
#define IVW_PROCESSORGRAPHICSITEM_H

#include <QGraphicsRectItem>

#include "inviwo/core/processors/processor.h"

#include "inviwo/qt/editor/networkeditor.h"

namespace inviwo {

class ProcessorGraphicsItem : public QGraphicsRectItem {
    //Q_OBJECT

public:
    ProcessorGraphicsItem();
    ~ProcessorGraphicsItem();

    Processor* getProcessor() const;
    std::string getIdentifier() const;

protected:
    virtual void setProcessor(Processor* processor);

    Port* getSelectedPort(const QPointF pos);


    bool event(QEvent* event);

    void paint(QPainter* p, const QStyleOptionGraphicsItem* opt, QWidget* w);
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    friend class NetworkEditor;
    friend class ConnectionGraphicsItem;

    Processor* processor_;

    QRectF calculatePortRect(unsigned int curPort, Port::PortDirection portDir);
    QRectF calculatePortRect(Port* port);

};

} // namespace

#endif // IVW_PROCESSORGRAPHICSITEM_H