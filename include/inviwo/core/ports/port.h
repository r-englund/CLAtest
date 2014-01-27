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

#ifndef IVW_PORT_H
#define IVW_PORT_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/properties/propertyowner.h>
#include <inviwo/core/util/callback.h>

namespace inviwo {

class Processor;
class MultiInport;

class IVW_CORE_API Port : public IvwSerializable {

friend class Processor;
friend class MultiInport;

public:
    /**
     * Constructor for creating port instances. As this class is abstract, the constructor is not
     * called directly. Instead, constructors of the derived classes call this constructor.
     *
     * @param identifier Port identifier used for serialization. Should be unique within the scope
     *                   of a processor.
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
    /**
     * All instances have the same color. 
     * Derived should declare its own and return DerivedClass::colorCode in getColorCode
     */
    static uvec3 colorCode;
    /**
     * Return true if the port which the pointer points to has the same color as this port.
     */
    bool equalColorCode(Port*) const;

    Processor* getProcessor() const;
    std::string getIdentifier() const;

    virtual std::string getClassName() const = 0;
    
	virtual bool isConnected() const = 0;
	virtual bool isReady() const = 0;

    virtual void invalidate(PropertyOwner::InvalidationLevel invalidationLevel);
    virtual PropertyOwner::InvalidationLevel getInvalidationLevel() const { return PropertyOwner::INVALID_OUTPUT; }
    virtual void setInvalidationLevel(PropertyOwner::InvalidationLevel invalidationLevel) = 0;

    template <typename T>
    void onChange(T* o, void (T::*m)()) {
        onChangeCallback_.addMemberFunction(o,m);
    }

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

protected:   
    std::string identifier_;

    void setIdentifier(const std::string& name);
    void setProcessor(Processor* processor);

    SingleCallBack onChangeCallback_;

private:
    Processor* processor_;
};

} // namespace

#endif // IVW_PORT_H
