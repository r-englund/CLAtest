#ifndef IVW_PORTINSPECTOR_H
#define IVW_PORTINSPECTOR_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/network/processornetwork.h>
#include <inviwo/core/ports/inport.h>
#include <inviwo/core/processors/canvasprocessor.h>
#include <string.h>



namespace inviwo {

class IVW_CORE_API PortInspector { 
public:
    PortInspector(std::string portClassName, std::string inspectorWorkspace);
    virtual ~PortInspector();

    std::string getInspectorWorkspace();
    std::string getPortClassName();
    ProcessorNetwork* getInspectorNetwork();

    std::vector<Inport*> getInports();
    CanvasProcessor* getCanvasProcessor();

private:
    void initialize();
    
    std::string inspectorNetworkFileName_;
    std::string portClassName_;
    ProcessorNetwork* inspectorNetwork_;
 
    std::vector<Inport*> inPorts_;
    CanvasProcessor* canvasProcessor_;

};

} // namespace

#endif // IVW_PORTINSPECTOR_H

