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

    void setIdentifier(const std::string& identifier);
    std::string getIdentifier() const;

    void setProcessorWidget(ProcessorWidget* processorWidget);
    ProcessorWidget* getProcessorWidget() const;
    bool hasProcessorWidget() const;

    MetaData* getMetaData(std::string meta);

    virtual void initialize();
    virtual void deinitialize();
    bool isInitialized() const;

    Port* getPort(std::string identifier) const;
    Inport* getInport(std::string identifier) const;
    Outport* getOutport(std::string identifier) const;

    const std::vector<Inport*>& getInports() const;
    const std::vector<Outport*>& getOutports() const;
    std::vector<std::string> getPortDependencySets() const;
    std::vector<Port*> getPortsByDependencySet(std::string portDependencySet) const;
    std::string getPortDependencySet(Port* port) const;

    bool allInportsConnected() const;
    bool allInportsAreReady() const;

    virtual bool isEndProcessor() const;
    virtual bool isReady() const;

    virtual void process();
    
    //Triggers invalidation. 
    //Perform only full reimplementation of this function, meaning never call Proccessor::invalidate()
    //in your reimplemented invalidation function.
    virtual void invalidate(PropertyOwner::InvalidationLevel invalidationLevel);
    
    virtual void setValid();
    virtual void initializeResources() {} // reload shaders etc. here

    void addInteractionHandler(InteractionHandler* interactionHandler);
    void removeInteractionHandler(InteractionHandler* interactionHandler);
    bool hasInteractionHandler() const;
	std::vector<InteractionHandler*> getInteractionHandlers() const;
    void invokeInteractionEvent(Event* event);

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

protected:
    void addPort(Inport* port, std::string portDependencySet="default");
    void addPort(Inport& port, std::string portDependencySet="default");

    void addPort(Outport* port, std::string portDependencySet="default");
    void addPort(Outport& port, std::string portDependencySet="default");

    virtual void performEvaluateRequest();

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
