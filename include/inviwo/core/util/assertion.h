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

#ifndef IVW_ASSERTION_H
#define IVW_ASSERTION_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <iostream>

IVW_CORE_API void ivwAssertion(const char* fileName, const char* functionName, long lineNumber, std::string message);

#if defined(_DEBUG)
#define ivwAssert(condition, message) \
{   std::ostringstream stream__; stream__ << message; \
    if (!(bool(condition))) ivwAssertion(__FILE__, __FUNCTION__, __LINE__, (stream__.str()));}
#else
#define ivwAssert(condition, message)
#endif

#endif // IVW_ASSERTION_H
