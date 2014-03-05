/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Main file authors: Erik Sundén, Timo Ropinski
 *
 *********************************************************************************/

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

    /**
     * Sets the identifier of the Processor. If there already exist a processor with that identifier it will append a number, starting at 2 to ensure uniqueness of identifiers.
     * @param the new identifier
     * @return The identifier that was set including eventual appended number
     */
    std::string setIdentifier(const std::string& identifier);
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
    virtual void invalidate(PropertyOwner::InvalidationLevel invalidationLevel, Property* modifiedProperty=0);

    virtual void setValid();
    virtual void initializeResources() {} // reload shaders etc. here

    /**
     * Adds the interaction handler such that it receives events propagated
     * to the processor. Will not add the interaction handler if it has been added before.
     * @note The processor will not take ownership of the interaction handler.
     * @see InteractionHandler
     * @param interactionHandler Interaction handler to be added.
     */
    void addInteractionHandler(InteractionHandler* interactionHandler);
    /**
     * Remove the interaction handler such that events are not propagated.
     * The interaction handler will not be deleted by the processor.
     * @param interactionHandler Interaction handler to be removed.
     */
    void removeInteractionHandler(InteractionHandler* interactionHandler);
    bool hasInteractionHandler() const;
    const std::vector<InteractionHandler*>& getInteractionHandlers() const;
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

    static std::set<std::string> usedIdentifiers_;

    bool initialized_;
};

} // namespace

#endif // IVW_PROCESSOR_H
