/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#ifndef IVW_NETWORKEDITOR_H
#define IVW_NETWORKEDITOR_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <QGraphicsScene>
#include <QTimer>
#include <QThread>

#include <inviwo/core/network/processornetworkevaluator.h>
#include <inviwo/core/processors/processorfactory.h>
#include <inviwo/core/ports/inport.h>
#include <inviwo/core/ports/outport.h>
#include <inviwo/core/util/singleton.h>
#include <inviwo/core/util/observer.h>

#include "processorgraphicsitem.h"
#include "connectiongraphicsitem.h"
#include "linkgraphicsitem.h"
#include "linkdialog.h"

namespace inviwo {

class NetworkEditorObserver : public Observer{
public:
    virtual void onNetworkEditorFileChanged(const std::string &newFilename) = 0;
};

/**
 * The NetworkEditor supports interactive editing of a ProcessorNetwork. Processors can be added
 * and removed, and their ports can be connected, while their properties can be linked.
 * - interactive editing, but if necessary use access functions
 * - graphical representation automatically managed
 * - inspector networks
 */
class IVW_QTEDITOR_API NetworkEditor : public QGraphicsScene,
                                       public Singleton<NetworkEditor>,
                                       public Observable<NetworkEditorObserver>         {
    Q_OBJECT
public:
    NetworkEditor();
    virtual ~NetworkEditor();

    /**
     * \brief This method adds a processor to the network.
     * - modified called by processornetwork
     * - models and views are added independently (e.g., addProcessor vs. addProcessorRepresentation)
     *
     * Before the processor is added to the network, its identifier is analyzed and
     * if necessary changed, such that the processor names within each network are unique.
     * 
     * @param Processor* processor The processor to be added
     * @param pos Position in the network editor, wher ethe graphical representation should be located
     * @param visible Shall the graphical representation be visible
     */
    void addProcessor(Processor* processor, QPointF pos, bool showProcessor=true, bool showPropertyWidgets=true, bool showProcessorWidget=true);
    void removeProcessor(Processor* processor);

    void addConnection(Outport* outport, Inport* inport);
    void removeConnection(Outport* outport, Inport* inport);

    void addLink(Processor* processor1, Processor* processor2);
    void removeLink(Processor* processor1, Processor* processor2);

    void clearNetwork();
    bool saveNetwork(std::string fileName);
    bool loadNetwork(std::string fileName);
    std::string getCurrentFilename()const{return filename_;}

    void addPropertyWidgets(Processor* processor);
    void removeAndDeletePropertyWidgets(Processor* processor);
    void removePropertyWidgets(Processor* processor);

    const ProcessorNetwork* getProcessorNetwork() const { return processorNetwork_; }
    ProcessorNetworkEvaluator* getProcessorNetworkEvaluator() const { return processorNetworkEvaluator_; }

    std::vector<std::string> getSnapshotsOfExternalNetwork(std::string fileName);

    void updateLinkGraphicsItems();

public slots:
    void hoverPortTimeOut();
    void workerThreadReset();
    void cacheProcessorProperty(Processor*);

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

	void placeProcessorOnConnection(ProcessorGraphicsItem* processorItem, ConnectionGraphicsItem* connectionItem);
    void placeProcessorOnProcessor(ProcessorGraphicsItem* processorItem, ProcessorGraphicsItem* oldProcessorItem);  

    bool isLinkDisplayEnabled();
    void autoLinkOnAddedProcessor(Processor*);

    void workerThreadQuit();

private:
    enum NetworkEditorFlags { 
        None=0, 
        CanvasHidden=1, 
        UseOriginalCanvasSize=1<<2
    };

    friend class ProcessorGraphicsItem;
    friend class ConnectionGraphicsItem;

    std::vector<ProcessorGraphicsItem*> processorGraphicsItems_;
    std::vector<ConnectionGraphicsItem*> connectionGraphicsItems_;
    std::vector<LinkConnectionGraphicsItem*> linkGraphicsItems_;

    ProcessorNetwork* processorNetwork_;
    ProcessorNetworkEvaluator* processorNetworkEvaluator_;

	ConnectionGraphicsItem* oldConnectionTarget_;
	ProcessorGraphicsItem* oldProcessorTarget_;

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
    QTimer hoverTimer_;
    QThread* workerThread_;

    void addProcessorRepresentations(Processor* processor, QPointF pos, bool showProcessor=true, bool showPropertyWidgets=true, bool showProcessorWidget=true);
    void removeProcessorRepresentations(Processor* processor);
    void addProcessorGraphicsItem(Processor* processor, QPointF pos, bool visible=true);
    void removeProcessorGraphicsItem(Processor* processor);
    void addProcessorWidget(Processor* processor, bool visible=true);
    void removeProcessorWidget(Processor* processor);

    void removeConnection(ConnectionGraphicsItem* connectionGraphicsItem);
    void addConnectionGraphicsItem(Outport* outport, Inport* inport);
    void removeConnectionGraphicsItem(ConnectionGraphicsItem* connectionGraphicsItem);

    void removeLink(LinkConnectionGraphicsItem* linkGraphicsItem);
    void addLinkGraphicsItem(Processor* processor1, Processor* processor2);
    void removeLinkGraphicsItem(LinkConnectionGraphicsItem* linkGraphicsItem);

    void showLinkDialog(LinkConnectionGraphicsItem* linkConnectionGraphicsItem);

    ProcessorGraphicsItem* getProcessorGraphicsItem(std::string identifier) const;
    ConnectionGraphicsItem* getConnectionGraphicsItem(Outport* outport, Inport* inport) const;
    LinkConnectionGraphicsItem* getLinkGraphicsItem(Processor* processor1, Processor* processor2) const;

    ProcessorGraphicsItem* getProcessorGraphicsItemAt(const QPointF pos) const;
    ConnectionGraphicsItem* getConnectionGraphicsItemAt(const QPointF pos) const;
    LinkConnectionGraphicsItem* getLinkGraphicsItemAt(const QPointF pos) const;

    void addPortInspector(Port* port, QPointF pos);
    void removePortInspector(Port*);

    void addExternalNetwork(std::string fileName, std::string processorPrefix, ivec2 pos, unsigned int networkEditorFlags=NetworkEditor::None, ivec2 canvasSize=ivec2(128));
    void removeExternalNetwork(std::string identifierPrefix);
    std::vector<std::string> saveSnapshotsInExternalNetwork(std::string externalNetworkFile, std::string identifierPrefix);

    QPointF snapToGrid(QPointF pos);
    void drawBackground(QPainter* painter, const QRectF& rect);
    std::string obtainUniqueProcessorID(std::string) const;

    std::string filename_;
};

class IVW_QTEDITOR_API ProcessorWorkerQt : public QObject{
    Q_OBJECT
public:
    ProcessorWorkerQt(std::vector<Processor*> processors) : processors_(processors){}
    ~ProcessorWorkerQt(){};

public slots:
    void process();

signals:
    void nextProcessor(Processor*);
    void finished();
    void error(QString err);

private:
    std::vector<Processor*> processors_;
};

} // namespace

#endif // IVW_NETWORKEDITOR_H