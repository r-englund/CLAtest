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

#ifndef IVW_GLUTMODULE_H
#define IVW_GLUTMODULE_H

#include "modules/glut/glutmoduledefine.h"
#include "inviwo/core/common/inviwomodule.h"

namespace inviwo {

class IVW_MODULE_GLUT_API GLUTModule : public InviwoModule {

public:
    GLUTModule();

};

} // namespace

#endif // IVW_GLUTMODULE_H
