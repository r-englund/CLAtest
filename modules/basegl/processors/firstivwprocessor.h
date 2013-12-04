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

#ifndef IVW_FIRSTIVWPROCESSOR_H
#define IVW_FIRSTIVWPROCESSOR_H

#include <modules/basegl/baseglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/properties/vectorproperties.h>
#include <modules/opengl/inviwoopengl.h>
#include <modules/opengl/processorgl.h>

namespace inviwo {

class IVW_MODULE_BASEGL_API FirstIvwProcessor : public ProcessorGL {
public:
    FirstIvwProcessor();
    
    InviwoProcessorInfo();

protected:
    virtual void process();

private:
    FloatVec3Property color_;
    ImageOutport outport_;
};

} // namespace

#endif // VRN_FIRSTIVWPROCESSOR_H
