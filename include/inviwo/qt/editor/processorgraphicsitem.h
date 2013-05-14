#ifndef IVW_PROCESSORGRAPHICSITEM_H
#define IVW_PROCESSORGRAPHICSITEM_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <QGraphicsRectItem>

#include <inviwo/core/processors/processor.h>
#include <inviwo/qt/editor/labelgraphicsitem.h>

namespace inviwo {

//TODO: Move this to common header later on (eg., BaseGraphicsItem.h )
static const qreal PROCESSORGRAPHICSITEM_DEPTH = 1.0f;
static const qreal CONNECTIONGRAPHICSITEM_DEPTH = 2.0f;
static const qreal LINKGRAPHICSITEM_DEPTH = 2.0f;

IVW_QTEDITOR_API enum InviwoUserGraphicsItemType {
    ProcessorGraphicsType = 1,
    ConnectionGraphicsType = 2,
    LinkGraphicsType = 3
    //LinkDialogGraphicsType = 4
};

class IVW_QTEDITOR_API ProcessorGraphicsItem : public QGraphicsRectItem, public VoidObserver {

public:
    ProcessorGraphicsItem();
    ~ProcessorGraphicsItem();

    Processor* getProcessor() const { return processor_; }
    std::string getIdentifier() const { return processor_->getIdentifier(); }
    void setProcessor(Processor* processor);
    void editProcessorName();
    QRectF calculatePortRect(Inport* port) const;
    QRectF calculatePortRect(Outport* port) const;
    QRectF calculateInportRect(size_t curPort) const;
    QRectF calculateOutportRect(size_t curPort) const;
    Port* getSelectedPort(const QPointF pos) const;
    void updateMetaData();
    void flipLayout();
    QPointF getShortestBoundaryPointTo(ProcessorGraphicsItem* processorGraphicsItem);
    QPointF getShortestBoundaryPointTo(QPointF inPos);

    void notify();

    //override for qgraphicsitem_cast (refer qt documentation)
    enum { Type = UserType + ProcessorGraphicsType };
    int type() const  {return Type; }

protected:
    void paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget);
    void paintStatusIndicator(QPainter* p, QPointF offset, bool status, QColor baseColor);
    void paintProgressBar(QPainter* p, float progress);
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    Processor* processor_;
    LabelGraphicsItem* nameLabel_;
    LabelGraphicsItem* classLabel_;
};

} // namespace

#endif // IVW_PROCESSORGRAPHICSITEM_H