/**********************************************************************
 * Copyright (C) 2104 ContextVision AB
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

#include <modules/cvie/cviemodule.h>
#include <modules/cvie/cvie2dimageprocessor.h>
#include <modules/cvie/cvie2dvolumeprocessor.h>

namespace inviwo {

CVIEModule::CVIEModule() : InviwoModule() {
    setIdentifier("CVIE");

    registerProcessor(CVIE2DImageProcessor);
    registerProcessor(CVIE2DVolumeProcessor);
	
}

} // namespace
             