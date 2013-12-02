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

namespace inviwo {

class Processor;

class IVW_CORE_API Port : public IvwSerializable {

friend class Processor;

public:
    /**
     * Constructor for creating port instances. As this class is abstract, the constructor is not
     * called directly. Instead, constructors of the derived classes call this constructor.
     *
     * @param identifier Port identifier used for serialization. Should be unique within the scope
     *                   of a processor.
     * @param invalidationLevel Defines the level of invalidation used upon connection/deconnection.
     * @see Processor::addPort(), PropertyOwner::InvalidationLevel
     */
    Port(std::string identifier,
         PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT);
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

    Processor* getProcessor() const;
    std::string getIdentifier() const;

	virtual bool isConnected() const = 0;
	virtual bool isReady() const = 0;

    virtual void invalidate(PropertyOwner::InvalidationLevel invalidationLevel);

    PropertyOwner::InvalidationLevel getInvalidationLevel() const { return invalidationLevel_; }
    virtual void setInvalidationLevel(PropertyOwner::InvalidationLevel invalidationLevel) {
        invalidationLevel_ = invalidationLevel;
    }

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

protected:
    std::string identifier_;
    PropertyOwner::InvalidationLevel invalidationLevel_;

    void setIdentifier(const std::string& name);
    void setProcessor(Processor* processor);

private:
    Processor* processor_;
};

} // namespace

#endif // IVW_PORT_H
