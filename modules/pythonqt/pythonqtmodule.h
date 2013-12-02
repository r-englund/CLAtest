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

#ifndef IVW_PYTHONQT_MODULE_H
#define IVW_PYTHONQT_MODULE_H

#include <modules/pythonqt/pythonqtmoduledefine.h>
#include <inviwo/core/common/inviwomodule.h>

namespace inviwo {
class IVW_MODULE_PYTHONQT_API PythonQtModule : public InviwoModule {
public:
    PythonQtModule();
    virtual ~PythonQtModule();
};

} // namespace

#endif // IVW_PYTHONQT_MODULE_H
