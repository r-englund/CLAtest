#ifndef IVW_NETWORKEDITOR_H
#define IVW_NETWORKEDITOR_H

#include <QGraphicsScene>

#include "inviwo/core/network/processornetworkevaluator.h"
#include "inviwo/core/processors/processorfactory.h"
#include "inviwo/core/ports/port.h"

#include "processorgraphicsitem.h"
#include "connectiongraphicsitem.h"
#include "linkgraphicsitem.h"
#include "linkdialog.h"

namespace inviwo {

class CanvasQt;

class NetworkEditor : public QGraphicsScene {

public:
    static NetworkEditor* instance();
    ~NetworkEditor();

    Processor* createProcessor(std::string className);
    void removeProcessor(std::string identifier);
    void initializeProcessorRepresentation(Processor* processor, QPointF pos=QPointF(10.0f, 10.0f));

    void initializeConnectionRepresentation(ProcessorGraphicsItem* outProcessor, Port* outport, 
                                       ProcessorGraphicsItem* inProcessor, Port* inport) ;

    void initializeLinkRepresentation(ProcessorGraphicsItem* outProcessor, ProcessorGraphicsItem* inProcessor);

    void addConnection(ProcessorGraphicsItem* startProcessor_, Port* startPort_,
                       ProcessorGraphicsItem* endProcessor_, Port* endPort_);

    void addConnection(PortConnection *connection);

    void addLink(ProcessorGraphicsItem* outProcessor, ProcessorGraphicsItem* inProcessor);

    void addLink(ProcessorLink *link);

    void removeConnection(ConnectionGraphicsItem* connectionGraphicsItem);

    void removeLink(LinkConnectionGraphicsItem* linkGraphicsItem);

    bool saveNetwork(std::string fileName);

    bool loadNetwork(std::string fileName);

    void clearNetwork();

    void setVerticalNetworkLayout(bool layoutOption);

    void drawBackground(QPainter* painter, const QRectF& rect);

    ProcessorNetworkEvaluator* getProcessorNetworkEvaluator();

    ProcessorGraphicsItem* getProcessorGraphicsItem(std::string identifier) const;
    QGraphicsItem* getProcessorGraphicsItemAt(const QPointF pos) const;
    QGraphicsItem* getConnectionGraphicsItemAt(const QPointF pos) const;
    QGraphicsItem* getLinkGraphicsItemAt(const QPointF pos) const;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* e);

    void keyPressEvent(QKeyEvent* keyEvent);

    void contextMenuEvent(QGraphicsSceneContextMenuEvent* e);

    void dragEnterEvent(QGraphicsSceneDragDropEvent* de);
    void dragMoveEvent(QGraphicsSceneDragDropEvent* de);
    void dropEvent(QGraphicsSceneDragDropEvent* de);

    NetworkEditor(QObject* parent=0);
    QPointF snapToGrid(QPointF pos);

private:
    friend class ProcessorGraphicsItem;
    friend class ConnectionGraphicsItem;

    std::vector<ProcessorGraphicsItem*> processorGraphicsItems_;
    std::vector<ConnectionGraphicsItem*> connectionGraphicsItems_;
    std::vector<LinkConnectionGraphicsItem*> linkGraphicsItems_;

    ProcessorNetwork* processorNetwork_;
    ProcessorNetworkEvaluator* processorNetworkEvaluator_;

    bool processorWithIdentifierExists(std::string identifier);
    
    CurveGraphicsItem* connectionCurve_;
    LinkGraphicsItem* linkCurve_;
    ProcessorGraphicsItem* startProcessor_;
    ProcessorGraphicsItem* endProcessor_;
    Port* startPort_;
    Port* endPort_;

    bool verticalLayout_;
    bool gridSnapping_;
    
    static const std::string logSource_; ///< Source string to be displayed for log messages.
};

} // namespace

#endif // IVW_NETWORKEDITOR_H