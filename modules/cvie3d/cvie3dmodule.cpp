/**********************************************************************
 * Copyright (C) 2013 Vistinct AB
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

#include <modules/cvie3d/cvie3dmodule.h>
#include <modules/cvie3d/cvie3dprocessor.h>

namespace inviwo {

CVIE3DModule::CVIE3DModule() : InviwoModule() {
    setIdentifier("CVIE3D");

    registerProcessor(CVIE3DProcessor);
	
}

} // namespace
             