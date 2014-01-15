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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#ifndef IVW_DATAREPRESENTATION_H
#define IVW_DATAREPRESENTATION_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {

class DataFormatBase;
class DataOperation;

class IVW_CORE_API DataRepresentation {

public:
    DataRepresentation();
    DataRepresentation(const DataFormatBase* format);
    DataRepresentation(const DataRepresentation& rhs);
    DataRepresentation& operator=(const DataRepresentation& that);
    virtual DataRepresentation* clone() const = 0;
    virtual ~DataRepresentation();

    virtual std::string getClassName() const;
    const DataFormatBase* getDataFormat() const;
    std::string getDataFormatString() const;
    DataFormatId getDataFormatId() const;
    virtual void performOperation(DataOperation*) const = 0;

protected:
    void setDataFormat(const DataFormatBase* format);

    const DataFormatBase* dataFormatBase_;
};

} // namespace

#endif // IVW_DATAREPRESENTATION_H
