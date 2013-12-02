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

#ifndef IVW_PORTGROUP_H
#define IVW_PORTGROUP_H

#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {

class ImageOutport;
class FrameBufferObject;
class Shader;

class IVW_MODULE_OPENGL_API PortGroup {

public:
    PortGroup();

    void initialize();
    void deinitialize();

    void activate();
    void deactivate();

    void addPort(ImageOutport& port);

    void reattachTargets();

    void addShaderDefines(Shader* shader);

private:
    FrameBufferObject* frameBufferObject_;

    std::vector<ImageOutport*>* ports_;
};

} // namespace

#endif // IVW_PORTGROUP_H
