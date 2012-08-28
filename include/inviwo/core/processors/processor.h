#ifndef IVW_PROCESSOR_H
#define IVW_PROCESSOR_H

#include "inviwo/core/inviwo.h"
#include "inviwo/core/interaction/interactionhandler.h"
#include "inviwo/core/interaction/events/event.h"
#include "inviwo/core/ports/port.h"
#include "inviwo/core/processors/processorwidget.h"
#include "inviwo/core/properties/properties.h"
#include "inviwo/core/properties/propertyowner.h"

namespace inviwo {

class Processor : public PropertyOwner, public Serializable {

public:
    Processor();
    virtual ~Processor();

    enum CodeState {
        CODE_STATE_EXPERIMENTAL,
        CODE_STATE_TESTING,
        CODE_STATE_STABLE
    };

    virtual std::string getClassName() const { return "Processor"; }
    virtual std::string getCategory() const { return "undefined"; }
    virtual CodeState getCodeState() const { return codeState_; }
    virtual Processor* create() const;

    void setIdentifier(const std::string& identifier) { identifier_ = identifier; }
    std::string getIdentifier() const { return identifier_; }

    virtual void createProcessorWidget() { processorWidget_ = 0; }
    void setProcessorWidget(ProcessorWidget* processorWidget) { processorWidget_ = processorWidget; }
    ProcessorWidget* getProcessorWidget() { return processorWidget_; }
    bool hasProcessorWidget() { return (processorWidget_ != 0); }

    virtual void initialize() throw (Exception);
    virtual void deinitialize() throw (Exception);

    virtual void beforeProcess();
    virtual void process();
    virtual void afterProcess();

    virtual bool isEndProcessor() { return false; }

    Port* getPort(std::string identifier) const;
    std::vector<Port*> getInports() { return inports_; }
    std::vector<Port*> getOutports() { return outports_; }

    bool allInportsConnected() const;

    void invalidate();
    bool isValid() { return PropertyOwner::isValid(); }
    void setValid() { PropertyOwner::setValid(); }

    void addInteractionHandler(InteractionHandler* interactionHandler);
    void removeInteractionHandler(InteractionHandler* interactionHandler);
    inline bool hasInteractionHandler() { return (interactionHandlers_.size() != 0); }
    void invokeInteractionEvent(Event* event);

protected:

    void addPort(Port* port);
    void addPort(Port& port);

    ProcessorWidget* processorWidget_;

    static const std::string logSource_; ///< Source string to be displayed for log messages.

private:
    CodeState codeState_;
    std::string identifier_;
    std::vector<Port*> inports_;
    std::vector<Port*> outports_;

    std::vector<InteractionHandler*> interactionHandlers_;

    virtual void serialize(XmlSerializer& s) const;
    virtual void deserialize(XmlDeserializer& s);
};

} // namespace

#endif // IVW_PROCESSOR_H
