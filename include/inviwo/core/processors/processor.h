#ifndef IVW_PROCESSOR_H
#define IVW_PROCESSOR_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/interaction/interactionhandler.h>
#include <inviwo/core/interaction/events/event.h>
#include <inviwo/core/ports/inport.h>
#include <inviwo/core/ports/outport.h>
#include <inviwo/core/processors/processorwidget.h>
#include <inviwo/core/properties/properties.h>
#include <inviwo/core/properties/propertyowner.h>
#include <inviwo/core/metadata/metadata.h>
#include <inviwo/core/util/group.h>
#include <inviwo/core/util/observer.h>

namespace inviwo {

class IVW_CORE_API Processor : public PropertyOwner, public VoidObservable {

public:
    Processor();
    virtual ~Processor();

    enum CodeState {
        CODE_STATE_BROKEN,
        CODE_STATE_EXPERIMENTAL,
        CODE_STATE_STABLE
    };

    virtual std::string getClassName() const { return "Processor"; }
    virtual std::string getCategory() const { return "undefined"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL; }
    virtual Processor* create() const;

    void setIdentifier(const std::string& identifier) { identifier_ = identifier; }
    std::string getIdentifier() const { return identifier_; }

    void setProcessorWidget(ProcessorWidget* processorWidget) { processorWidget_ = processorWidget; }
    ProcessorWidget* getProcessorWidget() { return processorWidget_; }
    bool hasProcessorWidget() { return (processorWidget_ != 0); }

    MetaData* getMetaData(std::string meta);

    virtual void initialize();
    virtual void deinitialize();

    virtual void process();

    virtual bool isEndProcessor() { return false; }

    Port* getPort(std::string identifier) const;
    std::vector<Inport*> getInports() { return inports_; }
    std::vector<Outport*> getOutports() { return outports_; }
    std::vector<std::string> getPortGroupNames();
    std::vector<Port*> getPortsByGroup(std::string groupName);
    std::string getPortGroupName(Port* port);

    bool allInportsConnected() const;

    // TODO: should validation mechanism be moved into PropertyOwner?
    void invalidate();
    bool isValid() { return PropertyOwner::isValid(); }
    void setValid() { PropertyOwner::setValid(); }
    InvalidationLevel getInvalidationLevel() { return invalidationLevel_; }
    void initializeResources() {} // TODO: reload shaders etc. here

    void showProgressBar() { showProgressBar_ = true; resetProgress(); }
    void hideProgressBar() { showProgressBar_ = false; resetProgress(); }
    bool hasProgressBar() const { return showProgressBar_; }
    float getProgress() const { return progress_; }
    void resetProgress() { progress_ = 0.0f; }
    void finishProgress() { progress_ = 1.0f; }

    void addInteractionHandler(InteractionHandler* interactionHandler);
    void removeInteractionHandler(InteractionHandler* interactionHandler);
    inline bool hasInteractionHandler() { return (interactionHandlers_.size() != 0); }
    void invokeInteractionEvent(Event* event);

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

protected:

    void addPort(Inport* port, std::string groupName="default");
    void addPort(Inport& port, std::string groupName="default");  

    void addPort(Outport* port, std::string groupName="default");
    void addPort(Outport& port, std::string groupName="default");  

    void updateProgress(float progress);
    void updateProgressLoop(size_t loopVar, size_t maxLoopVar, float endProgress);

    ProcessorWidget* processorWidget_;

private:
    std::string identifier_;

    std::vector<Inport*> inports_;
    std::vector<Outport*> outports_;

    std::vector<InteractionHandler*> interactionHandlers_;

    //TODO: Use map
    std::vector<MetaData*> metaData_; 

    Group<std::string,Port*> portGroup_;

    InvalidationLevel invalidationLevel_;

    bool showProgressBar_;
    float progress_;
    float beginLoopProgress_;
};

} // namespace

#endif // IVW_PROCESSOR_H
