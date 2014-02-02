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

#ifndef IVW_DATAREADERDIALOG_H
#define IVW_DATAREADERDIALOG_H

#include <inviwo/core/util/formats.h>

namespace inviwo {

class IVW_CORE_API DataReaderDialog {
public:
    DataReaderDialog();
    virtual ~DataReaderDialog();

    virtual const DataFormatBase* getFormat(std::string fileName, uvec3* dimensions, bool* endianess) = 0;
};

} // namespace

#endif // IVW_DATAREADERDIALOG_H
    
    