/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Main file authors: Timo Ropinski, Erik Sundén
 *
 *********************************************************************************/

#ifndef IVW_NETWORKEDITOR_H
#define IVW_NETWORKEDITOR_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <QGraphicsScene>
#include <QTimer>
#include <QThread>

#include <inviwo/core/network/processornetworkevaluator.h>
#include <inviwo/core/processors/processorfactory.h>
#include <inviwo/core/ports/inport.h>
#include <inviwo/core/ports/inspection.h>
#include <inviwo/core/ports/outport.h>
#include <inviwo/core/util/singleton.h>
#include <inviwo/core/util/observer.h>
#include <inviwo/qt/editor/portinfographicsitem.h>
#include <inviwo/qt/editor/processorgraphicsitem.h>
#include <inviwo/qt/editor/connectiongraphicsitem.h>
#include <inviwo/qt/editor/linkgraphicsitem.h>
#include <inviwo/qt/editor/linkdialog.h>

namespace inviwo {

class NetworkEditorObserver : public Observer {
public:
    virtual void onNetworkEditorFileChanged(const std::string& newFilename) = 0;
    virtual void onModifiedStatusChanged(const bool &newStatus) = 0;
};

/**
 * The NetworkEditor supports interactive editing of a ProcessorNetwork. Processors can be added
 * and removed, and their ports can be connected, while their properties can be linked.
 * - interactive editing, but if necessary use access functions
 * - graphical representation automatically managed
 * - inspector networks
 */
class IVW_QTEDITOR_API NetworkEditor 
    : public QGraphicsScene
    , public Singleton<NetworkEditor>
    , public Observable<NetworkEditorObserver>
    , public ProcessorNetworkObserver
{
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
    void addProcessor(Processor* processor, QPointF pos, bool showProcessor=true, bool selectProcessor=true, bool showPropertyWidgets=true,
                      bool showProcessorWidget=true);
    void removeProcessor(Processor* processor);

    void addConnection(Outport* outport, Inport* inport);
    void removeConnection(Outport* outport, Inport* inport);

    void addLink(Processor* processor1, Processor* processor2);
    void removeLink(Processor* processor1, Processor* processor2);

    void clearNetwork();
    bool saveNetwork(std::string fileName);
    
    /** 
     * Save network to stream. 
     * Uses current file name for relative paths.
     * 
     * @param std::ostream stream Stream to save data
     * @return bool true if successful, false otherwise.
     */
    bool saveNetwork(std::ostream stream);
    bool loadNetwork(std::string fileName);
    /**
     * Load network from a stream. The path will be used to calculate relative directories of data (nothing will be stored in the path). 
     * @param std::iostream& stream Stream with content that is to be deserialized.
     * @param std::string path A path that will be used to calculate location of data during deserialization.
     */
    bool loadNetwork(std::istream& stream, const std::string& path);

    std::string getCurrentFilename()const {return filename_;}

    void addPropertyWidgets(Processor* processor);
    void removeAndDeletePropertyWidgets(Processor* processor);
    void removePropertyWidgets(Processor* processor);

    const ProcessorNetwork* getProcessorNetwork() const { return processorNetwork_; }
    ProcessorNetworkEvaluator* getProcessorNetworkEvaluator() const { return processorNetworkEvaluator_; }

    std::vector<std::string> getSnapshotsOfExternalNetwork(std::string fileName);

    void updateLinkGraphicsItems();

    void renamingFinished();

    bool isModified()const;
    void setModified(const bool modified = true);

    virtual void onProcessorNetworkChange() {setModified();};

public slots:
    void managePortInspection();
    void workerThreadReset();
    void cacheProcessorProperty(Processor*);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* e);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* e);

    //called by mouse event callbacks to update the meta data states of processor graphics items
    void updateAllProcessorGraphicsItemMetaData();

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
    
    bool isPortInformationActive();

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
    PortInfoGraphicsItem* portInfoGraphicsItem_;
    CurveGraphicsItem* connectionCurve_;
    LinkGraphicsItem* linkCurve_;

    // these members are used to save the start state for click-move events
    ProcessorGraphicsItem* startProcessor_;
    ProcessorGraphicsItem* endProcessor_;
    Port* startPort_;
    Port* endPort_;

    BoolProperty* portInformationActive_;
    Inspection inspection_;

    bool gridSnapping_;
    static const int GRID_SPACING;
    QTimer hoverTimer_;
    QThread* workerThread_;

    void addProcessorRepresentations(Processor* processor, QPointF pos, bool showProcessor=true, bool selectProcessor=true,
                                     bool showPropertyWidgets=true,
                                     bool showProcessorWidget=true);
    void removeProcessorRepresentations(Processor* processor);
    void addProcessorGraphicsItem(Processor* processor, QPointF pos, bool visible=true, bool selected=true);
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

    void addPortInspector(std::string processorIdentifier, std::string portIdentifier, QPointF pos);
    void removePortInspector(std::string processorIdentifier, std::string portIdentifier);

    void addPortInformation(std::string processorIdentifier, std::string portIdentifier, std::string portInformation, QPointF pos);
    void removePortInformation();

    void addExternalNetwork(std::string fileName, std::string processorPrefix, ivec2 pos, unsigned int networkEditorFlags=NetworkEditor::None,
                            ivec2 canvasSize=ivec2(128));
    void removeExternalNetwork(std::string identifierPrefix);
    std::vector<std::string> saveSnapshotsInExternalNetwork(std::string externalNetworkFile, std::string identifierPrefix);

    QPointF snapToGrid(QPointF pos);
    void drawBackground(QPainter* painter, const QRectF& rect);

    std::string filename_;
    bool renamingProcessor_;
    bool modified_;
};

class IVW_QTEDITOR_API ProcessorWorkerQt : public QObject {
    Q_OBJECT
public:
    ProcessorWorkerQt(std::vector<Processor*> processors) : processors_(processors) {}
    ~ProcessorWorkerQt() {};

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
