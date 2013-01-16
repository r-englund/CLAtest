#ifndef IVW_PROCESSORGRAPHICSITEM_H
#define IVW_PROCESSORGRAPHICSITEM_H

#include "inviwo/qt/editor/inviwoqteditordefine.h"
#include <QGraphicsRectItem>

#include "inviwo/core/processors/processor.h"
#include "inviwo/qt/editor/labelgraphicsitem.h"

namespace inviwo {

//TODO: Move this to common header later on (eg., BaseGraphicsItem.h )
static const qreal PROCESSORGRAPHICSITEM_DEPTH = 1.0f;
static const qreal CONNECTIONGRAPHICSITEM_DEPTH = 2.0f;
static const qreal LINKGRAPHICSITEM_DEPTH = 3.0f;

IVW_QTEDITOR_API enum InviwoUserGraphicsItemType {
    ProcessorGraphicsType = 1,
    ConnectionGraphicsType = 2,
    LinkGraphicsType = 3
    //LinkDialogGraphicsType = 4
};

class IVW_QTEDITOR_API ProcessorGraphicsItem : public QGraphicsRectItem {

public:
    ProcessorGraphicsItem();
    ~ProcessorGraphicsItem();

    Processor* getProcessor() const { return processor_; }
    std::string getIdentifier() const { return processor_->getIdentifier(); }
    void setProcessor(Processor* processor);
    void editProcessorName();
    QRectF calculatePortRect(Port* port) const;
    QRectF calculatePortRect(unsigned int curPort, Port::PortDirection portDir) const;
    Port* getSelectedPort(const QPointF pos) const;
    void updateMetaData();
    void flipLayout();
    QPointF getShortestBoundaryPointTo(ProcessorGraphicsItem* processorGraphicsItem);
    QPointF getShortestBoundaryPointTo(QPointF inPos);

    //override for qgraphicsitem_cast (refer qt documentation)
    enum { Type = UserType + ProcessorGraphicsType };
    int type() const  {return Type; }

protected:
    void paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget);
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    Processor* processor_;
    LabelGraphicsItem* nameLabel_;
    LabelGraphicsItem* classLabel_;
};

} // namespace

#endif // IVW_PROCESSORGRAPHICSITEM_H