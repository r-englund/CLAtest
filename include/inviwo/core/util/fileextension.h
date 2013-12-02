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
 * Primary author : Peter Steneteg
 *
 **********************************************************************/

#ifndef IVW_FILEEXTENSION_H
#define IVW_FILEEXTENSION_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {

class IVW_CORE_API FileExtension { 
public:
    FileExtension();
    FileExtension(std::string extension, std::string description);
    virtual ~FileExtension() {};

    std::string extension_;
    std::string description_;
};

} // namespace

#endif // IVW_FILEEXTENSION_H

