#ifndef IVW_NETWORKEDITOR_H
#define IVW_NETWORKEDITOR_H

#include <QGraphicsScene>

#include "inviwo/core/network/processornetworkevaluator.h"
#include "inviwo/core/processors/processorfactory.h"
#include "inviwo/core/ports/port.h"

#include "processorgraphicsitem.h"
#include "connectiongraphicsitem.h"

namespace inviwo {

class NetworkEditor : public QGraphicsScene {

public:
    static NetworkEditor* instance();
    ~NetworkEditor();

    void addProcessor(std::string className, QPointF pos=QPointF(10.0f, 10.0f), Processor* processor=0);
    void removeProcessor(std::string identifier);

    void addConnection(ProcessorGraphicsItem* startProcessor_, Port* startPort_,
                       ProcessorGraphicsItem* endProcessor_, Port* endPort_);
    void removeConnection(ConnectionGraphicsItem* connectionGraphicsItem);

    bool saveNetwork(std::string fileName);

    bool loadNetwork(std::string fileName);

    ProcessorGraphicsItem* getProcessorGraphicsItem(std::string identifier) const;
    QGraphicsItem* getGraphicsItemAt(const QPointF pos) const;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* e);

    void contextMenuEvent(QGraphicsSceneContextMenuEvent* e);

    void dragEnterEvent(QGraphicsSceneDragDropEvent* de);
    void dragMoveEvent(QGraphicsSceneDragDropEvent* de);
    void dropEvent(QGraphicsSceneDragDropEvent* de);

protected:
    NetworkEditor(QObject* parent=0);

private:
    friend class ProcessorGraphicsItem;
    friend class ConnectionGraphicsItem;

    std::vector<ProcessorGraphicsItem*> processorGraphicsItems_;
    std::vector<ConnectionGraphicsItem*> connectionGraphicsItems_;

    ProcessorNetwork* processorNetwork_;
    ProcessorNetworkEvaluator* processorNetworkEvaluator_;

    bool processorWithIdentifierExists(std::string identifier);
    
    CurveGraphicsItem* connectionCurve_;
    ProcessorGraphicsItem* startProcessor_;
    ProcessorGraphicsItem* endProcessor_;
    Port* startPort_;
    Port* endPort_;

    static const std::string logSource_; ///< Source string to be displayed for log messages.
};

} // namespace

#endif // IVW_NETWORKEDITOR_H