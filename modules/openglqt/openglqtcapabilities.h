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
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#ifndef IVW_OPENGLQTCAPABILITIES_H
#define IVW_OPENGLQTCAPABILITIES_H

#include <modules/openglqt/openglqtmoduledefine.h>
#include <inviwo/core/util/capabilities.h>
#include <modules/opengl/openglcapabilities.h>

namespace inviwo {

class IVW_MODULE_OPENGLQT_API OpenGLQtCapabilities : public Capabilities  {
public:    
    OpenGLQtCapabilities();
    virtual ~OpenGLQtCapabilities();
    void printInfo();
    virtual void retrieveStaticInfo() {};
    virtual void retrieveDynamicInfo() {};
    std::vector<int> getGLVersion();
};

} // namespace

#endif // IVW_OPENGLQTCAPABILITIES_H
