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

#ifndef IVW_DATAWRITER_H
#define IVW_DATAWRITER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include "inviwo/core/datastructures/data.h"

namespace inviwo {

class IVW_CORE_API DataWriter {

public:
    DataWriter();
    virtual ~DataWriter();

    std::string getIdentifier() const;
    virtual void writeData()=0;

protected:

    void setIdentifier(const std::string& name);

private:
    std::string identifier_;
};

} // namespace

#endif // IVW_DATAWRITER_H
