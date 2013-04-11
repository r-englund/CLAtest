#ifndef IVW_PROCESSORNETWORK_H
#define IVW_PROCESSORNETWORK_H

#include <inviwo/core/inviwo.h>
#include <inviwo/core/inviwocoredefine.h>

#include <inviwo/core/processors/processor.h>
#include <inviwo/core/network/portconnection.h>
#include <inviwo/core/network/processorlink.h>
#include <inviwo/core/util/observer.h>

namespace inviwo {

/**
 * This class manages the current processor network. It can be thought of as a container of
 * \sa Processors, which \sa Ports are connected through \sa PortConnections, and which
 * \sa Properties are linked through \sa ProcessorLinks. To manage \sa Processors,
 * \sa PortConnections and \sa ProcessorLinks, add and remove methods are available for all
 * these entities. The network should be only modified by using these methods. Typically,
 * these methods are called by the \sa NetworkEditor, which enables the user to edit
 * \sa ProcessorNetworks.
 *
 * When the \sa PorcessorNetwork has been changed, it is flagged as modified. This mechanism is
 * used by the \sa ProcessorNetworkEvaluator to identify if the \sa ProcessorNetwork needs to be
 * analyzed before processing.
 *
 * In the model view controller design pattern, the \sa ProcessorNetwork represents the model,
 * which means that no graphical representations are generated for the added entities. Adding
 * and removing of the graphical representations is done in the \sa NetworkEditor.
 */
class IVW_CORE_API ProcessorNetwork : public IvwSerializable, public VoidObservable {

public:

    ProcessorNetwork();
    virtual ~ProcessorNetwork();

    /**
     * Adds a \sa Processor to the \sa ProcessorNetwork. The identifiers of all processors in the
     * \sa ProcessorNetwork should be unique.
     *
     * @param processor The \sa Processor to be added.
     * @see removeProcessor(), Processor::setIdentifier()
     */
    void addProcessor(Processor* processor);

    /**
     * Removes a \sa Processor from the \sa ProcessorNetwork. To ensure that the network does not end up
     * in a corrupt state, this method first removes all \sa PortConnections and \sa ProcessorLinks, which
     * are related to the \sa Processor to be removed.
     *
     * @param processor The \sa Processor to be removed.
     * @see addProcessor()
     */
    void removeProcessor(Processor* processor);


    /**
     * Adds a \sa PortConnection to the \sa ProcessorNetwork. This involves creating the connection
     * between the two specified ports, as well as adding this connection to the \sa ProcessorNetwork.
     * The order in which the two \sa Ports are specified does not play a role.
     *
     * @param port1 One of the two \sa Ports to be connected.
     * @param port2 The other of the two \sa Ports to be connected.
     * @see removeConnection()
     */
    void addConnection(Port* port1, Port* port2);

    /**
     * Removes a \sa PortConnection from the \sa ProcessorNetwork. This involves resolving the connection
     * between the two specified \sa Ports, as well as removing this connection from the
     * \sa ProcessorNetwork. The order in which the two \sa Ports are specified does not play a role.
     *
     * @param port1 One of the two \sa Ports to be connected.
     * @param port2 The other of the two \sa Ports to be connected.
     * @see addConnection()
     */
    void removeConnection(Port* port1, Port* port2);

    void addLink(Processor* sourceProcessor, Processor* destProcessor);
    void removeLink(Processor* sourceProcessor, Processor* destProcessor);

    std::vector<Processor*> getProcessors() const { return processors_; }
    std::vector<PortConnection*> getPortConnections() const { return portConnections_; }
    std::vector<ProcessorLink*> getProcessorLinks() const { return processorLinks_; }


    /**
     * Returns the \sa Processor from the \sa ProcessorNetwork, which has the given identifier.
     * In case no \sa Processor with the given identifier is contained in the network, a null
     * pointer is returned.
     *
     * @param identifier Identifier of the \sa Processor to be accessed.
     * @see getProcessorsByType(), Processor::setIdentifier(), Processor::getIdentifier()
     */
    Processor* getProcessorByName(std::string identifier) const;

    /**
     * Returns a vector of \sa Processors which are of type T. In case no \sa Processors match T
     * an empty vector is returned.
     *
     * @see getProcessorByName()
     */
    template<class T> std::vector<T*> getProcessorsByType() const;

    ProcessorLink* getProcessorLink(Processor* sourceProcessor, Processor* destProcessor) const;    
    
    inline void modified() { isModified_ = true; notifyObservers(); }
    void setModified(bool isModified) { isModified_ = isModified; }
    bool isModified() const { return isModified_; }

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& s);

protected:
    static const std::string logSource_; ///< Source string to be displayed for log messages.

private:
    bool isModified_;

    std::vector<Processor*> processors_;
    std::vector<PortConnection*> portConnections_;
    std::vector<ProcessorLink*> processorLinks_;

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
