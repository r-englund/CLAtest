#include "inviwo/core/network/processornetwork.h"

//TODO: remove these includes
#include "../modules/base/processors/firstivwprocessor.h"
#include "../modules/base/processors/imagesource.h"
#include "../modules/base/processors/entryexitpoints.h"
#include "../modules/base/processors/simpleraycaster.h"
#include "../modules/base/processors/imagesource.h"
#include "../modules/opengl/canvasprocessorgl.h"
#include "inviwo/core/processors/datasource/volumesource.h"

namespace inviwo {

    // FIXME: temporary method supporting non serialization-based network creation
    void ProcessorNetwork::connectPorts(Port* sourcePort, Port* destPort) {
        destPort->connectTo(sourcePort);
        portConnections_.push_back(new PortConnection(sourcePort, destPort));
    }

    ProcessorNetwork::ProcessorNetwork() {
        /*
        FirstIvwProcessor* firstProcessor = new FirstIvwProcessor();
        firstProcessor->setIdentifier("FirstIVWProcessor");
        CanvasProcessorGL* canvasProcessor = new CanvasProcessorGL();
        canvasProcessor->setIdentifier("CanvasProcessor");
        processors_.push_back(firstProcessor);
        processors_.push_back(canvasProcessor);
        connectPorts(firstProcessor->getPort("outport"), canvasProcessor->getPort("inport"));
        */

        /*
        ImageSource* imageSource = new ImageSource();
        imageSource->setIdentifier("ImageSource");
        FirstIvwProcessor* firstProcessor = new FirstIvwProcessor();
        firstProcessor->setIdentifier("FirstIVWProcessor");
        CanvasProcessorGL* canvasProcessor = new CanvasProcessorGL();
        canvasProcessor->setIdentifier("CanvasProcessor");
        processors_.push_back(imageSource);
        processors_.push_back(firstProcessor);
        processors_.push_back(canvasProcessor);
        connectPorts(imageSource->getPort("image.outport"), firstProcessor->getPort("image.inport"));
        connectPorts(firstProcessor->getPort("image.outport"), canvasProcessor->getPort("image.inport"));
        */

        /*
        // simple volume raycasting
        VolumeSource* volumeSource = new VolumeSource();
        volumeSource->setIdentifier("VolumeSource");
        EntryExitPoints* entryExitPoints = new EntryExitPoints();
        entryExitPoints->setIdentifier("EntryExitPoints");
        SimpleRaycaster* rayCaster = new SimpleRaycaster();
        rayCaster->setIdentifier("SimpleRaycaster");
        CanvasProcessorGL* canvasProcessorGL = new CanvasProcessorGL();
        canvasProcessorGL->setIdentifier("CanvasProcessor");
        processors_.push_back(volumeSource);
        processors_.push_back(entryExitPoints);
        processors_.push_back(rayCaster);
        processors_.push_back(canvasProcessorGL);
        connectPorts(volumeSource->getPort("volume"), entryExitPoints->getPort("volume"));
        connectPorts(volumeSource->getPort("volume"), rayCaster->getPort("volume"));
        connectPorts(entryExitPoints->getPort("entry-points"), rayCaster->getPort("entry-points"));
        connectPorts(entryExitPoints->getPort("exit-points"), rayCaster->getPort("exit-points"));
        connectPorts(rayCaster->getPort("outport"), canvasProcessorGL->getPort("inport"));
        */
        
        /*
        // show entry points
        VolumeSource* volumeSource = new VolumeSource();
        volumeSource->setIdentifier("VolumeSource");
        EntryExitPoints* entryExitPoints = new EntryExitPoints();
        entryExitPoints->setIdentifier("EntryExitPoints");
        CanvasProcessorGL* canvasProcessorGL = new CanvasProcessorGL();
        canvasProcessorGL->setIdentifier("CanvasProcessor");
        processors_.push_back(volumeSource);
        processors_.push_back(entryExitPoints);
        processors_.push_back(canvasProcessorGL);
        connectPorts(volumeSource->getPort("volume"), entryExitPoints->getPort("volume"));
        connectPorts(entryExitPoints->getPort("entry-points"), canvasProcessorGL->getPort("inport"));
        */

        /*
        // show exit points
        VolumeSource* volumeSource = new VolumeSource();
        volumeSource->setIdentifier("VolumeSource");
        EntryExitPoints* entryExitPoints = new EntryExitPoints();
        entryExitPoints->setIdentifier("EntryExitPoints");
        CanvasProcessorGL* canvasProcessorGL = new CanvasProcessorGL();
        canvasProcessorGL->setIdentifier("CanvasProcessor");
        processors_.push_back(volumeSource);
        processors_.push_back(entryExitPoints);
        processors_.push_back(canvasProcessorGL);
        connectPorts(volumeSource->getPort("volume"), entryExitPoints->getPort("volume"));
        connectPorts(entryExitPoints->getPort("exit-points"), canvasProcessorGL->getPort("inport"));
        */
    }

    ProcessorNetwork::~ProcessorNetwork() {}

    void ProcessorNetwork::addProcessor(Processor* processor) {
        if(std::find(processors_.begin(), processors_.end(), processor)== processors_.end())
            processors_.push_back(processor);
    }

    void ProcessorNetwork::clear() {
        while(processors_.size()) {
            removeProcessor(processors_.front()) ;
        }
    }

    void ProcessorNetwork::removeProcessor(Processor* processor) {
        // remove all connections to other processors
        std::vector<PortConnection*> eraseList;
        std::vector<Port*> outports = processor->getOutports();
        for (size_t i=0; i<portConnections_.size(); i++) {
            for (size_t j=0; j<outports.size(); j++) {
                if (portConnections_[i]->getOutport() == outports[j]) {
                    outports[j]->disconnectFrom(portConnections_[i]->getInport());
                    eraseList.push_back(portConnections_[i]);
                }
            }
        }
        for (size_t i=0; i<eraseList.size(); i++) {
            for (size_t j=0; j<portConnections_.size(); j++) {
                if (portConnections_[j] == eraseList[i])
                    portConnections_.erase(portConnections_.begin()+j);
            }
        }

        eraseList.clear();
        std::vector<Port*> inports = processor->getInports();
        for (size_t i=0; i<portConnections_.size(); i++) {
            for (size_t j=0; j<inports.size(); j++) {
                if (portConnections_[i]->getInport() == inports[j]) {
                    inports[j]->disconnectFrom(portConnections_[i]->getOutport());
                    eraseList.push_back(portConnections_[i]);
                }
            }
        }
        for (size_t i=0; i<eraseList.size(); i++) {
            for (size_t j=0; j<portConnections_.size(); j++) {
                if (portConnections_[j] == eraseList[i])
                    portConnections_.erase(portConnections_.begin()+j);
            }
        }

        // remove processors itself
        processors_.erase(std::remove(processors_.begin(), processors_.end(),
                          processor), processors_.end());
    }

    Processor* ProcessorNetwork::getProcessorByName(std::string name) const {
        for (size_t i=0; i<processors_.size(); i++)
            if ((processors_[i]->getIdentifier()).compare(name) == 0)
                return processors_[i];
        return 0;
        // TODO: throw not found exception
    }

    void ProcessorNetwork::serialize(IvwSerializer& s) const {
        s.serialize("Processors", processors_, "Processor");
        s.serialize("Connections", portConnections_, "Connection");
    }

    void ProcessorNetwork::deserialize(IvwDeserializer& d) {
        clear();
        d.deserialize("Processors", processors_, "Processor");
        d.deserialize("Connections", portConnections_, "Connection");
    }

} // namespace
