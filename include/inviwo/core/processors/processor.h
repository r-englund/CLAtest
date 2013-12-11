/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#ifndef IVW_PROCESSOR_H
#define IVW_PROCESSOR_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/interaction/interactionhandler.h>
#include <inviwo/core/interaction/events/event.h>
#include <inviwo/core/ports/inport.h>
#include <inviwo/core/ports/outport.h>
#include <inviwo/core/processors/processorstate.h>
#include <inviwo/core/processors/processorwidget.h>
#include <inviwo/core/properties/properties.h>
#include <inviwo/core/properties/propertyowner.h>
#include <inviwo/core/metadata/metadata.h>
#include <inviwo/core/util/group.h>
#include <inviwo/core/processors/processorobserver.h>

namespace inviwo {

#define InviwoProcessorInfo() \
        virtual std::string getClassName() const { return CLASS_NAME; } \
        virtual std::string getCategory() const { return CATEGORY; } \
        virtual CodeState getCodeState() const { return CODE_STATE; } \
        static const std::string CLASS_NAME; \
        static const std::string CATEGORY; \
        static const CodeState CODE_STATE;

#define ProcessorClassName(T, className) \
    const std::string T::CLASS_NAME = className;

#define ProcessorCategory(T, category) \
    const std::string T::CATEGORY = category;

#define ProcessorCodeState(T, codeState) \
    const CodeState T::CODE_STATE = codeState;

class IVW_CORE_API Processor : public PropertyOwner, public ProcessorObservable {

public:
    Processor();
    virtual ~Processor();
   
    InviwoProcessorInfo();

    void setIdentifier(const std::string& identifier) { identifier_ = identifier; }
    std::string getIdentifier() const { return identifier_; }

    void setProcessorWidget(ProcessorWidget* processorWidget) { processorWidget_ = processorWidget; }
    ProcessorWidget* getProcessorWidget() { return processorWidget_; }
    bool hasProcessorWidget() { return (processorWidget_ != 0); }

    MetaData* getMetaData(std::string meta);

    virtual void initialize();
    virtual void deinitialize();
    bool isInitialized() { return initialized_; }

    virtual void process();

    virtual bool isEndProcessor() { return false; }

    Port* getPort(std::string identifier);
    Inport* getInport(std::string identifier);
    Outport* getOutport(std::string identifier);

    std::vector<Inport*> getInports() { return inports_; }
    std::vector<Outport*> getOutports() { return outports_; }
    std::vector<std::string> getPortDependencySets();
    std::vector<Port*> getPortsByDependencySet(std::string portDependencySet);
    std::string getPortDependencySet(Port* port);

    bool allInportsConnected() const;
	bool allInportsAreReady() const;

    virtual bool isReady() const { return allInportsAreReady(); }

    //Triggers invalidation. 
    //Perform only full reimplementation of this function, meaning never call Proccessor::invalidate()
    //in your reimplemented invalidation function.
    virtual void invalidate(PropertyOwner::InvalidationLevel invalidationLevel);
    
    virtual void setValid();
    virtual void initializeResources() {} // reload shaders etc. here

    void addInteractionHandler(InteractionHandler* interactionHandler);
    void removeInteractionHandler(InteractionHandler* interactionHandler);
    inline bool hasInteractionHandler() { return (interactionHandlers_.size() != 0); }
	std::vector<InteractionHandler*> getInteractionHandlers() const { return interactionHandlers_; }
    void invokeInteractionEvent(Event* event);

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

protected:

    void addPort(Inport* port, std::string portDependencySet="default");
    void addPort(Inport& port, std::string portDependencySet="default");

    void addPort(Outport* port, std::string portDependencySet="default");
    void addPort(Outport& port, std::string portDependencySet="default");

    ProcessorWidget* processorWidget_;

private:
    std::string identifier_;

    std::vector<Inport*> inports_;
    std::vector<Outport*> outports_;

    std::vector<InteractionHandler*> interactionHandlers_;

    //TODO: Use map
    std::vector<MetaData*> metaData_; 

    Group<std::string, Port*> portDependencySets_;

    bool initialized_;
};

} // namespace

#endif // IVW_PROCESSOR_H
