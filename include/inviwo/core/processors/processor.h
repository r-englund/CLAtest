#ifndef IVW_PROCESSOR_H
#define IVW_PROCESSOR_H

#include "inviwo/core/inviwo.h"
#include "inviwo/core/interaction/interactionhandler.h"
#include "inviwo/core/interaction/events/event.h"
#include "inviwo/core/ports/port.h"
#include "inviwo/core/processors/processorwidget.h"
#include "inviwo/core/properties/properties.h"
#include "inviwo/core/properties/propertyowner.h"
#include "inviwo/core/metadata/metadata.h"
#include "inviwo/core/util/group.h"

namespace inviwo {

class Processor : public PropertyOwner {

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

    MetaData* getMetaData(std::string meta);

    virtual void initialize() throw (Exception);
    virtual void deinitialize() throw (Exception);

    virtual void beforeProcess();
    virtual void process();
    virtual void afterProcess();

    virtual bool isEndProcessor() { return false; }

    Port* getPort(std::string identifier) const;
    std::vector<Port*> getInports() { return inports_; }
    std::vector<Port*> getOutports() { return outports_; }
    std::vector<std::string> getPortGroupNames();
    std::vector<Port*> getPortsByGroup(std::string groupName);
    std::string getPortGroupName(Port* port);
    

    bool allInportsConnected() const;

    void invalidate();
    bool isValid() { return PropertyOwner::isValid(); }
    void setValid() { PropertyOwner::setValid(); }

    void addInteractionHandler(InteractionHandler* interactionHandler);
    void removeInteractionHandler(InteractionHandler* interactionHandler);
    inline bool hasInteractionHandler() { return (interactionHandlers_.size() != 0); }
    void invokeInteractionEvent(Event* event);

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

protected:

    void addPort(Port* port, std::string groupName="default");
    void addPort(Port& port, std::string groupName="default");  

    ProcessorWidget* processorWidget_;

    static const std::string logSource_; ///< Source string to be displayed for log messages.

private:
    CodeState codeState_;
    std::string identifier_;
    std::vector<Port*> inports_;
    std::vector<Port*> outports_;

    std::vector<InteractionHandler*> interactionHandlers_;

    //TODO: Use map
    std::vector<MetaData*> metaData_;

    //typedef std::multimap<std::string,Port*> PortGroupMap;
    //PortGroupMap portGroupMap_;

    Group<std::string,Port*> portGroupMap_;
};

} // namespace

#endif // IVW_PROCESSOR_H
