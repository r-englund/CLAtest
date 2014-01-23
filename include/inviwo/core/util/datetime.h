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
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#ifndef IVW_DATETIME_H
#define IVW_DATETIME_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>

#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>

namespace inviwo {

STARTCLANGIGNORE("-Wunused-function")
static const std::string currentDateTime() {
    time_t currentDateTime = time(0);
    struct tm dateTimeStruct;
    char dateTimeBuffer[20];
    dateTimeStruct = *localtime(&currentDateTime);
    strftime(dateTimeBuffer, sizeof(dateTimeBuffer), "%Y%m%d_%H%M%S", &dateTimeStruct);
    return dateTimeBuffer;
}
ENDCLANGIGNORE

} // namespace

#endif // IVW_DATETIME_H
