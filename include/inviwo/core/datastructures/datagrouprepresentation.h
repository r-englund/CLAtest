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

#ifndef IVW_DATAGROUPREPRESENTATION_H
#define IVW_DATAGROUPREPRESENTATION_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/data.h>
#include <inviwo/core/datastructures/datarepresentation.h>

namespace inviwo {

/** \brief The base class for all DataGroupRepresentation objects.
 *
 *  It has reference to zero or many DataRepresentation objects, but never owns them,
 *  they are always owned by the Data object.
 *
 *  Differences between DataGroupRepresentation and DataRepresentation:
 *    - DataGroupRepresentation does not own DataRepresentation, does should never delete them.
 *    - DataGroupRepresentation becomes invalid when a child DataRepresentation is invalid.
 */

class DataGroup;

class IVW_CORE_API DataGroupRepresentation : public DataRepresentation {

friend class DataGroup;

public:
    DataGroupRepresentation();
    DataGroupRepresentation(const DataGroupRepresentation& rhs);
    DataGroupRepresentation& operator=(const DataGroupRepresentation& that);
    virtual DataGroupRepresentation* clone() const = 0;
    virtual ~DataGroupRepresentation();

    virtual std::string getClassName() const;
    virtual void performOperation(DataOperation*) const = 0;

    virtual void initialize() = 0;
    virtual void deinitialize() = 0;

protected:
    //Update representations_ with DataRepresentation from each Data and DataGroup object
    virtual void update(bool) = 0;
    virtual void setPointerToOwner(DataGroup*) = 0;
};

} // namespace

#endif // IVW_DATAGROUPREPRESENTATION_H
