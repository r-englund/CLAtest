/**********************************************************************
 * Copyright (C) 2014 ContextVision AB
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Daniel Jönsson
 *
 **********************************************************************/

#ifndef IVW_CV_ULTRASOUNDSHADING_MODULE_H
#define IVW_CV_ULTRASOUNDSHADING_MODULE_H

#include <modules/cvultrasoundshading/cvultrasoundshadingmoduledefine.h>
#include <inviwo/core/common/inviwomodule.h>

namespace inviwo {

class IVW_MODULE_CVULTRASOUNDSHADING_API CVUltrasoundShadingModule: public InviwoModule {

public:
    CVUltrasoundShadingModule();
	virtual ~CVUltrasoundShadingModule();
    void initialize();

};

} // namespace

#endif // IVW_CV_ULTRASOUNDSHADING_MODULE_H
