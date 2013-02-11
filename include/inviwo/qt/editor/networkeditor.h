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

class IVW_QTEDITOR_API NetworkEditor : public QGraphicsScene,
                                       public Singleton<NetworkEditor>  {
    public:

    NetworkEditor(QObject* parent=0);

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

    void addProcessorWidget(Processor* processor);
    void addPropertyWidgets(Processor* processor);
    void removePropertyWidgets(Processor* processor);

    void addProcessorGraphicsItem(Processor* processor, QPointF pos=QPointF(10.0f, 10.0f));
    void removeProcessorGraphicsItem(Processor* processor);

    void addConnectionGraphicsItem(ProcessorGraphicsItem* outProcessor, Port* outport,
                                   ProcessorGraphicsItem* inProcessor, Port* inport);
    void removeConnectionGraphicsItem(ConnectionGraphicsItem* connectionGraphicsItem);

    void addLinkGraphicsItem(ProcessorGraphicsItem* outProcessor, ProcessorGraphicsItem* inProcessor);
    void removeLinkGraphicsItem(LinkConnectionGraphicsItem* linkGraphicsItem);


    void addInspectorNetwork(Port* port, ivec2 pos, std::string fileName);
    void removeInspectorNetwork(Port* port);
    void addPortInspector(Port* port, QPointF pos);


        bool saveNetwork(std::string fileName);
        bool loadNetwork(std::string fileName);
        void clearNetwork();

        void drawBackground(QPainter* painter, const QRectF& rect);

    ProcessorNetwork* getProcessorNetwork() const { return processorNetwork_; }
    ProcessorNetworkEvaluator* getProcessorNetworkEvaluator() const { return processorNetworkEvaluator_; }

        ProcessorGraphicsItem* getProcessorGraphicsItem(std::string identifier) const;
        QGraphicsItem* getProcessorGraphicsItemAt(const QPointF pos) const;
        QGraphicsItem* getConnectionGraphicsItemAt(const QPointF pos) const;
        QGraphicsItem* getLinkGraphicsItemAt(const QPointF pos) const;

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

        QPointF snapToGrid(QPointF pos);

        void showLinkDialog(LinkConnectionGraphicsItem* linkConnectionGraphicsItem);

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

        bool gridSnapping_;

        static const std::string logSource_; ///< Source string to be displayed for log messages.
    };

} // namespace

#endif // IVW_NETWORKEDITOR_H