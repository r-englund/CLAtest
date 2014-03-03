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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#include <modules/cvsc/cvscmodule.h>
#include <modules/cvsc/cvscprocessor.h>

namespace inviwo {

CVSCModule::CVSCModule() : InviwoModule() {
    setIdentifier("CVSC");

    registerProcessor(CVSCProcessor);
	
}

} // namespace
             