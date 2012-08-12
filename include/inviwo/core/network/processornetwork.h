#ifndef IVW_PROCESSORNETWORK_H
#define IVW_PROCESSORNETWORK_H

#include "inviwo/core/inviwo.h"
#include "inviwo/core/network/portconnection.h"
#include "inviwo/core/processors/processor.h"

namespace inviwo {

    class ProcessorNetwork : public Serializable {

public:

    // FIXME: temporary method supporting non serialization-based network creation
    void connectPorts(Port* sourcePort, Port* destPort);
    
    ProcessorNetwork();
    virtual ~ProcessorNetwork();

    void addProcessor(Processor* processor);
    void removeProcessor(Processor* processor);


    std::vector<Processor*> getProcessors() const { return processors_; }
    Processor* getProcessorByName(std::string name) const;
    template<class T> std::vector<T*> getProcessorsByType() const;

    std::vector<PortConnection*> getPortConnections() const { return portConnections_; }

    virtual void serialize(XmlSerializer& s) const;
    virtual void deserialize(XmlDeserializer& s);

private:

    std::vector<Processor*> processors_;
    std::vector<PortConnection*> portConnections_;

};

template<class T>
std::vector<T*> ProcessorNetwork::getProcessorsByType() const {
    std::vector<T*> processors;
    for (size_t i=0; i<processors_.size(); i++) {
        T* processor = dynamic_cast<T*>(processors_[i]);
        if (processor) processors.push_back(processor);
    }
    return processors;
}

} // namespace

#endif // IVW_PROCESSORNETWORK_H
