#ifndef IVW_OUTPORT_H
#define IVW_OUTPORT_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/ports/port.h>

namespace inviwo {

class Inport;

class IVW_CORE_API Outport : public Port {

friend class Inport;

public:
    Outport(std::string identifier);
    virtual ~Outport();

    bool isConnected() const;
    bool isConnectedTo(Inport* port) const;

    void invalidate();
    virtual std::vector<Processor*> getDescendantProcessors();

protected:
    void connectTo(Inport* port);
    void disconnectFrom(Inport* port);
    
    template <typename T>
    void getDescendantProcessorsUsingPortType(std::vector<Processor*>& descendantProcessors) {
        for (size_t i=0; i<connectedInports_.size(); i++) {
            Processor* decendantProcessor = connectedInports_[i]->getProcessor();

            if (std::find(descendantProcessors.begin(), descendantProcessors.end(), decendantProcessor)== descendantProcessors.end())
                descendantProcessors.push_back(connectedInports_[i]->getProcessor());

            std::vector<Outport*> outports = decendantProcessor->getOutports();
            for (size_t j=0; j<outports.size(); j++) {
                T* imageOutPort = dynamic_cast<T*>(outports[j]);
                if (imageOutPort)
                    imageOutPort->getDescendantProcessorsUsingPortType<T>(descendantProcessors);            
            }
        }
    }

private:
    std::vector<Inport*> connectedInports_;
};

} // namespace

#endif // IVW_OUTPORT_H