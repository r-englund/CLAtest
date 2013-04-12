#ifndef IVW_NETWORKEDITOR_H
#define IVW_NETWORKEDITOR_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <QGraphicsScene>

#include <inviwo/core/network/processornetworkevaluator.h>
#include <inviwo/core/processors/processorfactory.h>
#include <inviwo/core/ports/port.h>
#include <inviwo/core/util/singleton.h>

#include "processorgraphicsitem.h"
#include "connectiongraphicsitem.h"
#include "linkgraphicsitem.h"
#include "linkdialog.h"

namespace inviwo {

/**
 * The NetworkEditor supports interactive editing of a ProcessorNetwork. Processors can be added
 * and removed, and their ports can be connected, while their properties can be linked.
 * - interactive editing, but if necessary use access fucntions
 * - graphical representation automatically managed
 * - inspector networks
 */
class IVW_QTEDITOR_API NetworkEditor : public QGraphicsScene,
                                       public Singleton<NetworkEditor>  {
public:
    NetworkEditor(QObject* parent=0);

    void clearNetwork();
    bool saveNetwork(std::string fileName);
    bool loadNetwork(std::string fileName);


    /**
     * \brief This method adds a processor to the network.
     *
     * Before the processor is added to the network, its identifier is analyzed and
     * if necessary changed, such that the processor names within each network are unique.
     * 
     * @param Processor * processor The processor to be added
     */
    void addProcessor(Processor* processor);
    void removeProcessor(Processor* processor);

    void addConnection();
    void removeConnection();

    void addLink();
    void removeLink();


    ProcessorNetwork* getProcessorNetwork() const { return processorNetwork_; }
    ProcessorNetworkEvaluator* getProcessorNetworkEvaluator() const { return processorNetworkEvaluator_; }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* e);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* e);

    void keyPressEvent(QKeyEvent* keyEvent);

    void contextMenuEvent(QGraphicsSceneContextMenuEvent* e);

    void dragEnterEvent(QGraphicsSceneDragDropEvent* de);
    void dragMoveEvent(QGraphicsSceneDragDropEvent* de);
    void dropEvent(QGraphicsSceneDragDropEvent* de);

private:
    friend class ProcessorGraphicsItem;
    friend class ConnectionGraphicsItem;

    std::vector<ProcessorGraphicsItem*> processorGraphicsItems_;
    std::vector<ConnectionGraphicsItem*> connectionGraphicsItems_;
    std::vector<LinkConnectionGraphicsItem*> linkGraphicsItems_;

    ProcessorNetwork* processorNetwork_;
    ProcessorNetworkEvaluator* processorNetworkEvaluator_;

    CurveGraphicsItem* connectionCurve_;
    LinkGraphicsItem* linkCurve_;

    // these members are used to save the start state for click-move events
    ProcessorGraphicsItem* startProcessor_;
    ProcessorGraphicsItem* endProcessor_;
    Port* startPort_;
    Port* endPort_;
    Port* inspectedPort_; // the port which is currently inspected by hovering over it

    bool gridSnapping_;
    static const int GRID_SPACING;

    static const std::string logSource_; ///< Source string to be displayed for log messages.


    QPointF snapToGrid(QPointF pos);
    void drawBackground(QPainter* painter, const QRectF& rect);

    void showLinkDialog(LinkConnectionGraphicsItem* linkConnectionGraphicsItem);

    void addProcessorGraphicsItem(Processor* processor, QPointF pos=QPointF(10.0f, 10.0f));
    void removeProcessorGraphicsItem(Processor* processor);
    void addProcessorWidget(Processor* processor);
    void addPropertyWidgets(Processor* processor);
    void removePropertyWidgets(Processor* processor);

    void addConnectionGraphicsItem(ProcessorGraphicsItem* outProcessor, Port* outport,
                                   ProcessorGraphicsItem* inProcessor, Port* inport);
    void removeConnectionGraphicsItem(ConnectionGraphicsItem* connectionGraphicsItem);

    void addLinkGraphicsItem(ProcessorGraphicsItem* outProcessor, ProcessorGraphicsItem* inProcessor);
    void removeLinkGraphicsItem(LinkConnectionGraphicsItem* linkGraphicsItem);


    ProcessorGraphicsItem* getProcessorGraphicsItem(std::string identifier) const;
    QGraphicsItem* getProcessorGraphicsItemAt(const QPointF pos) const;
    QGraphicsItem* getConnectionGraphicsItemAt(const QPointF pos) const;
    QGraphicsItem* getLinkGraphicsItemAt(const QPointF pos) const;


    void addInspectorNetwork(Port* port, ivec2 pos, std::string fileName);
    void removeInspectorNetwork(Port* port);
    void addPortInspector(Port* port, QPointF pos);
};

} // namespace

#endif // IVW_NETWORKEDITOR_H