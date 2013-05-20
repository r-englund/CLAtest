#ifndef IVW_INPORT_H
#define IVW_INPORT_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/ports/port.h>

namespace inviwo {

class Outport;

class IVW_CORE_API Inport : public Port {

public:
    Inport(std::string identifier);
    virtual ~Inport();

    virtual void connectTo(Outport* outport);
    virtual void disconnectFrom(Outport* outport);

    bool isConnected() const;
    bool isConnectedTo(Outport* outport) const;

    void invalidate();
    virtual std::vector<Processor*> getAscendantProcessors();

protected:
    template <typename T>
    void getAscendantProcessorsUsingPortType(std::vector<Processor*>& ascendantProcessors) {
        if (connectedOutport_) {
            Processor* ascendantProcessor = connectedOutport_->getProcessor();

            if (std::find(ascendantProcessors.begin(), ascendantProcessors.end(), ascendantProcessor)== ascendantProcessors.end())
                ascendantProcessors.push_back(ascendantProcessor);

            std::vector<Inport*> inports = ascendantProcessor->getInports();
            for (size_t j=0; j<inports.size(); j++) {
                T* inPort = dynamic_cast<T*>(inports[j]);
                if (inPort)
                    inPort->getAscendantProcessorsUsingPortType<T>(ascendantProcessors);            
            }
        }
    }
private:
    Outport* connectedOutport_;
};

} // namespace

#endif // IVW_INPORT_H