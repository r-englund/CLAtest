#ifndef IVW_PORT_H
#define IVW_PORT_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/inviwo.h>

namespace inviwo {

    class Processor;

    class IVW_CORE_API Port {

    public:

        enum PortDirection {
            OUTPORT = 0,
            INPORT = 1
        };

        Port(PortDirection direction, std::string name);
        virtual ~Port();

        std::string getIdentifier() const {return identifier_; }
        void setProcessor(Processor* processor) { processor_ = processor; } //TODO: make private
        Processor* getProcessor() const { return processor_; }

        bool isOutport() const { return (direction_ == Port::OUTPORT); }
        bool isInport() const { return (direction_ == Port::INPORT); }
        bool isConnected() const { return connected_; }
        virtual void connectTo(Port* port);
        virtual void disconnectFrom(Port* port);

        virtual void initialize();
        virtual void deinitialize();

        void invalidate();

        virtual uvec3 getColorCode() { return uvec3(128,128,128); }

    protected:

        void setIdentifier(const std::string& name);

        PortDirection direction_;
        std::string identifier_;
        bool initialized_;
        bool connected_;

    private:
        Processor* processor_;
        std::vector<Port*> connectedPorts_;
    };

} // namespace

#endif // IVW_PORT_H
