/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#ifndef IVW_GEOMETRYPORT_H
#define IVW_GEOMETRYPORT_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/ports/datainport.h>
#include <inviwo/core/ports/dataoutport.h>
#include <inviwo/core/ports/multidatainport.h>
#include <inviwo/core/datastructures/geometry/geometry.h>

namespace inviwo {

class IVW_CORE_API GeometryInport : public DataInport<Geometry> {

public:
    GeometryInport(std::string identifier,
                   PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT);
    virtual ~GeometryInport();

    void initialize();
    void deinitialize();

    uvec3 getColorCode() const;
    static uvec3 colorCode;
};

class IVW_CORE_API GeometryMultiInport : public MultiDataInport<Geometry> {

public:
	GeometryMultiInport(std::string identifier);
	virtual ~GeometryMultiInport();

	void initialize();
	void deinitialize();

	uvec3 getColorCode() const;
};

class IVW_CORE_API GeometryOutport : public DataOutport<Geometry> {

public:
    GeometryOutport(std::string identifier,
                    PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT);
    virtual ~GeometryOutport();

    void initialize();
    void deinitialize();

    uvec3 getColorCode() const;
};

} // namespace

#endif // IVW_GEOMETRYPORT_H
