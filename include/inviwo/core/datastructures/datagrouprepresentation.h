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

class IVW_CORE_API DataGroupRepresentation : DataRepresentation {

    friend class DataGroup;

public:
    DataGroupRepresentation();
    DataGroupRepresentation(const DataGroupRepresentation& rhs);
    virtual ~DataGroupRepresentation();

    virtual void initialize() = 0;
    virtual void deinitialize() = 0;
    virtual DataGroupRepresentation* clone() const = 0;
    virtual std::string getClassName() const;

    //Update representations_ with DataRepresentation from each Data and DataGroup object
    virtual void update() = 0;

    //Function for checking if representations_ is identical (and valid) to those in the Data and DataGroup object.
    virtual bool isValid() const = 0;

protected:
    void setPointerToData(const std::vector<Data*>*, const std::vector<DataGroup*>*);

    std::vector<DataRepresentation*> representations_;

private:
    const std::vector<Data*>* data_;
    const std::vector<DataGroup*>* groupData_;
};

} // namespace

#endif // IVW_DATAGROUPREPRESENTATION_H
