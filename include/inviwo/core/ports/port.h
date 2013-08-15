#ifndef IVW_PORT_H
#define IVW_PORT_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/properties/propertyowner.h>

namespace inviwo {

class Processor;

class IVW_CORE_API Port : public IvwSerializable {

friend class Processor;

public:
    /**
     * Constructor for creating port instances. As this class is abstract, the constructor is not
     * called directly. Instead, constructors of the derived classes call this constructor.
     *
     * @param direction Defines if this is an INPORT or an OUTPORT.
     * @param identifier Port identifier used for serialization. Should be unique within the scope
                         of a processor.
     * @see Processor::addPort()
     */
    Port(std::string identifier);
    virtual ~Port();

    /**
     * Initializes the port. Here we can for instance initialize data to be stored inside the port.
     * As this method is pure virtual, it needs to be implemented within derived classes that should
     * be instantiated.
     * @see deinitialize()
     */
    virtual void initialize() = 0;

    /**
     * Deinitializes the port by reverting the initialization performed in initialize().
     * As this method is pure virtual, it needs to be implemented within derived classes that should
     * be instantiated.
     * @see initialize()
     */
    virtual void deinitialize() = 0;

    /**
     * Returns the RGB color code used to colorize all ports of this type. This color code is for
     * instance used in the NetworkEditor. To distinguish different port types through their color,
     * this method should be overloaded in derived classes.
     */
    virtual uvec3 getColorCode() const;

	virtual bool canConnectTo(Port* port) const { return false; };

    Processor* getProcessor() const;
    std::string getIdentifier() const;

    virtual bool isConnected() const = 0;

    virtual void invalidate(PropertyOwner::InvalidationLevel invalidationLevel);

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

protected:
    std::string identifier_;

    void setIdentifier(const std::string& name);
    void setProcessor(Processor* processor);

private:
    Processor* processor_;
};

} // namespace

#endif // IVW_PORT_H
