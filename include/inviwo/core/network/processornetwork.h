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
 * Main file authors: Timo Ropinski, Sathish Kottravel, Erik Sundén
 *
 *********************************************************************************/

#ifndef IVW_PROCESSORNETWORK_H
#define IVW_PROCESSORNETWORK_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/processors/processorobserver.h>
#include <inviwo/core/network/portconnection.h>
#include <inviwo/core/network/processornetworkobserver.h>
#include <inviwo/core/links/processorlink.h>
#include <inviwo/core/links/propertylink.h>
#include <inviwo/core/links/linkevaluator.h>
#include <inviwo/core/util/observer.h>
#include <inviwo/core/util/exception.h>

namespace inviwo {

/**
 * This class manages the current processor network. It can be thought of as a container of
 * Processor instances, which Port instances are connected through PortConnection instances,
 * and which Property instances are linked through ProcessorLink instances. To manage Processors,
 * PortConnections and ProcessorLinks, add and remove methods are available for all
 * these entities. The network should be only modified by using these methods. Typically,
 * these methods are called by the NetworkEditor, which enables the user to edit
 * ProcessorNetworks.
 *
 * When the PorcessorNetwork has been changed, it is flagged as modified. This mechanism is
 * used by the ProcessorNetworkEvaluator to identify if the ProcessorNetwork needs to be
 * analyzed before processing.
 *
 * In the model view controller design pattern, the ProcessorNetwork represents the model,
 * which means that no graphical representations are generated for the added entities. Adding
 * and removing of the graphical representations is done in the NetworkEditor.
 */
class IVW_CORE_API ProcessorNetwork : public IvwSerializable, public ProcessorNetworkObservable, public ProcessorObserver {

public:

    ProcessorNetwork();
    virtual ~ProcessorNetwork();

    /**
     * Adds a Processor to the ProcessorNetwork. The identifiers of all processors in the
     * ProcessorNetwork should be unique.
     *
     * @param[in] processor The Processor to be added.
     * @see removeProcessor(), Processor::setIdentifier()
     */
    void addProcessor(Processor* processor);

    /**
     * Removes a Processor from the ProcessorNetwork. To ensure that the network does not end up
     * in a corrupt state, this method first removes and deletes all PortConnections and ProcessorLinks, which
     * are related to the Processor to be removed.
     *
     * @param[in] processor The Processor to be removed.
     * @see addProcessor()
     */
    void removeProcessor(Processor* processor);

    /**
    * Removes and deletes a Processor from the ProcessorNetwork. To ensure that the network does not end up
    * in a corrupt state, this method first removes and deletes all PortConnections and ProcessorLinks, which
    * are related to the Processor to be removed.
    *
    * @param[in] processor The Processor to be removed.
    * @see addProcessor()
    */
    void removeAndDeleteProcessor(Processor* processor);

    /**
    * Returns the Processor from the ProcessorNetwork, which has the given identifier.
    * In case no Processor with the given identifier is contained in the network, a null
    * pointer is returned.
    *
    * @param identifier Identifier of the Processor to be accessed.
    * @see getProcessorsByType(), Processor::setIdentifier(), Processor::getIdentifier()
    */
    Processor* getProcessorByName(std::string identifier) const;

    /**
    * Returns a vector of Processors which are of type T. In case no Processors match T
    * an empty vector is returned.
    *
    * @see getProcessorByName()
    */
    template<class T> std::vector<T*> getProcessorsByType() const;

    /**
    * Returns a vector of all Processors.
    *
    * @return A vector of Processors
    */
    std::vector<Processor*> getProcessors() const;


    /**
    * Adds a PortConnection to the ProcessorNetwork. This involves creating the connection
    * between the two specified ports, as well as adding this connection to the ProcessorNetwork.
    *
    * @param[in] sourcePort The outport.
    * @param[in] destPort The inport.
    * @return The newly created connection. NULL if a connection could not be made.
    * @see removeConnection()
    */
    PortConnection* addConnection(Outport* sourcePort, Inport* destPort);

    /**
     * Removes and deletes a PortConnection from the ProcessorNetwork. This involves resolving the connection
     * between the two specified Ports, as well as removing this connection from the
     * ProcessorNetwork.
     *
     * @param[in] sourcePort The outport.
     * @param[in] destPort The inport.
     * @see addConnection()
     */
    void removeConnection(Outport* sourcePort, Inport* destPort);

    /**
    * Checks weather two port are connected
    *
    * @param[in] sourcePort The outport.
    * @param[in] destPort The inport.
    * @return Weather the two port are connected
    * @see addConnection()
    */
    bool isConnected(Outport* sourcePort, Inport* destPort);

    /**
    * Get a connection between two ports
    *
    * @param[in] sourcePort The outport.
    * @param[in] destPort The inport.
    * @return The PortConnection between the ports or NULL if there is none.
    * @see addConnection()
    */
    PortConnection* getConnection(Outport* sourcePort, Inport* destPort);

    /**
    * Returns a vector of all Connections.
    *
    * @return A vector of Connections
    */
    std::vector<PortConnection*> getConnections() const;

    ///////////////////////////////////////////////////////////////////////
    //TODO: ProcessorLinks are Deprecated. To be removed
    ProcessorLink* addLink(PropertyOwner* sourceProcessor, PropertyOwner* destProcessor);
    void removeLink(PropertyOwner* sourceProcessor, PropertyOwner* destProcessor);
    bool isLinked(PropertyOwner* src, PropertyOwner* dst);
    ProcessorLink* getProcessorLink(PropertyOwner* sourceProcessor, PropertyOwner* destProcessor) const;
    std::vector<ProcessorLink*> getProcessorLinks() const;
    ///////////////////////////////////////////////////////////////////////
    
    /** 
     * Create and add Property Link to the network
     *
     * Adds a link between two properties, that are owned by processor network.
     * 
     * @param[in] sourceProperty Property at which link starts
     * @param[in] destinationProperty Property at which link ends
     * @return PropertyLink* Newly added link
     */
    PropertyLink* addLink(Property* sourceProperty, Property* destinationProperty);
    /** 
     * Remove and delete Property Link from the network
     *
     * Removes a link between two properties, that are owned by processor network.
     * 
     * @param[in] sourceProperty Property at which link starts
     * @param[in] destinationProperty Property at which link ends
     * @return void 
     */
    void removeLink(Property* sourceProperty, Property* destinationProperty);
    /** 
     * Check whether Property Link exists
     *
     * Checks if there is a link between two properties, that are owned by processor network.
     * 
     * @param[in] sourceProperty Property at which link starts
     * @param[in] destinationProperty Property at which link ends
     * @return bool true if link exists otherwise returns false
     */
    bool isLinked(Property* sourceProperty, Property* destinationProperty);
    /** 
     * Find Property Link
     *
     * Search and return link between two properties, that are owned by processor network.
     * 
     * @param[in] sourceProperty Property at which link starts
     * @param[in] destinationProperty Property at which link ends
     * @return PropertyLink* returns pointer to link if it exists otherwise returns NULL
     */
    PropertyLink* getLink(Property* sourceProperty, Property* destinationProperty) const;
    /** 
     * Return all Property Links
     * 
     * @return std::vector<PropertyLink*> List of all property links owned by processor network
     */
    std::vector<PropertyLink*> getLinks() const;
    /** 
     * Remove bidirectional Property Link
     *
     * Searches for bidirectional link between start and end properties
     * In other words property that goes from end to start and removes it
     * 
     * @param[in] sourceProperty Property at which link starts
     * @param[in] destinationProperty Property at which link ends
     * @return void
     */
    void removeBidirectionalPair(Property* sourceProperty, Property* destinationProperty);
    /** 
     * Get bidirectional Property Link
     *
     * Searches for bidirectional link between start and end properties
     * In other words property that goes from end to start
     * 
     * @param[in] sourceProperty Property at which link starts
     * @param[in] destinationProperty Property at which link ends
     * @return void
     */
    PropertyLink* getBidirectionalPair(Property* sourceProperty, Property* destinationProperty);
    /** 
     * Modify or invalidate property of links
     *
     * Modify or invalidate property of a link where the property belongs to a given property owner
     * 
     * @param[in] processor given property owner
     * @return void
     */
    void setLinkModifiedByOwner(PropertyOwner* processor);
    /** 
     * Get all property links between two processors, though direction is not accounted
     * 
     * @param[in] sourceProcessor start processor
     * @param[in] destinationProcessor end processor
     * @return std::vector<PropertyLink*> List of all property links between sourceProcessor and destinationProcessor
     */
    std::vector<PropertyLink*> getLinksBetweenProcessors(PropertyOwner* sourceProcessor, PropertyOwner* destinationProcessor);
    /** 
     * Properties that are linked to the given property where the given property is a source property
     *
     * @param property given property
     * @return std::vector<Property*> List of all properties that are affected by given property
     */
    std::vector<Property*> getLinkedProperties(Property* property);

    void modified();
    void setModified(bool modified);
    bool isModified() const;

    bool isInvalidating() const;
    void onAboutPropertyChange(Property*);
    void onProcessorInvalidationBegin(Processor*);
    void onProcessorInvalidationEnd(Processor*);
    void onProcessorRequestEvaluate(Processor* p = NULL);
    Processor* getInvalidationInitiator();

    inline void lock() { locked_++; }
    inline void unlock() { (locked_>0) ? locked_-- : locked_=0;notifyProcessorNetworkUnlockedObservers(); }
    inline bool islocked() const { return (locked_!=0); }

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d) throw (Exception);

    bool isDeserializing()const;
    void updatePropertyLinkCaches();


    /**
    * Clears the network objects processors, port connections, property links etc.,
    * This function clears only the core objects and mainly used to abort any
    * further operation.
    */
    void clear();


private:
    //Property Linking support
    void performLinkingOnPropertyChange(Property* modifiedProperty);
    void evaluatePropertyLinks(Property*);
    void addToPrimaryCache(PropertyLink* propertyLink);
    void removeFromPrimaryCache(PropertyLink* propertyLink);
    void clearSecondaryCache();

    static const int processorNetworkVersion_;

    ///////////////////////////////////////////////////////////////////////
    //TODO: ProcessorLinks are Deprecated. To be removed
    std::vector<ProcessorLink*> getSortedProcessorLinksFromProperty(Property*);
    void evaluatePropertyLinks1(Property*);
    std::vector<ProcessorLink*> processorLinks_;
    ///////////////////////////////////////////////////////////////////////

    std::map<Property*, std::vector<Property*> > propertyLinkPrimaryCache_;
    std::map<Property*, std::vector<Property*> > propertyLinkSecondaryCache_;
    bool modified_;
    unsigned int locked_;
    std::vector<Processor*> processors_;
    std::vector<PortConnection*> portConnections_;
    std::vector<PropertyLink*> propertyLinks_;

    bool deserializing_;
    bool invalidating_;
    std::vector<Processor*> processorsInvalidating_;
    LinkEvaluator* linkEvaluator_;

    bool evaluationQueued_;

    bool linking_;
    Processor* linkInvalidationInitiator_;


    class NetworkConverter : public VersionConverter {
    public:
        typedef void (NetworkConverter::*updateType)(TxElement*);
        NetworkConverter(int from);
        virtual bool convert(TxElement* root);
        int from_;

    private:

        void updateProcessorType(TxElement* node);
        void traverseNodes(TxElement* node, updateType update);
    };

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
