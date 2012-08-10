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
        ImageSource* imageSource = new ImageSource();
        imageSource->setIdentifier("ImageSource");
        FirstIvwProcessor* firstProcessor = new FirstIvwProcessor();
        firstProcessor->setIdentifier("FirstIVWProcessor");
        CanvasProcessor* canvasProcessor = new CanvasProcessor();
        canvasProcessor->setIdentifier("CanvasProcessor");
        processors_.push_back(imageSource);
        processors_.push_back(firstProcessor);
        processors_.push_back(canvasProcessor);
        connectPorts(imageSource->getPort("image.outport"), firstProcessor->getPort("image.inport"));
        connectPorts(firstProcessor->getPort("image.outport"), canvasProcessor->getPort("image.inport"));
        */

        
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
    }

    ProcessorNetwork::~ProcessorNetwork() {}

    Processor* ProcessorNetwork::getProcessorByName(std::string name) const {
        for (size_t i=0; i<processors_.size(); i++)
            if ((processors_[i]->getIdentifier()).compare(name) == 0)
                return processors_[i];
        return 0;
        // TODO: throw not found exception
    }

    void ProcessorNetwork::serialize(XmlSerializer& /*s*/) const {}
    void ProcessorNetwork::deserialize(XmlDeserializer& /*d*/) {}

} // namespace
