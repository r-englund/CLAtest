#ifndef IVW_PORTINSPECTOR_H
#define IVW_PORTINSPECTOR_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/network/processornetwork.h>
#include <inviwo/core/network/portconnection.h>
#include <inviwo/core/ports/inport.h>
#include <inviwo/core/processors/canvasprocessor.h>
#include <string.h>



namespace inviwo {

class IVW_CORE_API PortInspector {
public:
    PortInspector(std::string portClassName, std::string inspectorWorkspace);
    virtual ~PortInspector();

    std::string getInspectorNetworkFileName();
    std::string getPortClassName();
    void setActive(bool val);
    bool isActive();

    std::vector<Processor*> getProcessors();
    std::vector<Inport*> getInports();
    CanvasProcessor* getCanvasProcessor();
    std::vector<PortConnection*> getConnections();
    std::vector<ProcessorLink*> getProcessorLinks();

private:
    void initialize();

    std::string inspectorNetworkFileName_;
    std::string portClassName_;
    bool active_;
    ProcessorNetwork* inspectorNetwork_;

    std::vector<Processor*> processors_;
    std::vector<Inport*> inPorts_;
    std::vector<PortConnection*> connections_;
    std::vector<ProcessorLink*> processorLinks_;
    CanvasProcessor* canvasProcessor_;

};

} // namespace

#endif // IVW_PORTINSPECTOR_H

