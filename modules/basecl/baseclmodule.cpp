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
 * Primary author : Daniel Jönsson
 *
 **********************************************************************/

#include <modules/basecl/baseclmodule.h>

#include <modules/basecl/processors/entryexitpointscl.h>
#include <modules/basecl/processors/volumefirsthitcl.h>

namespace inviwo {

BaseCLModule::BaseCLModule() : InviwoModule() {
    setIdentifier("BaseCL");
    setXMLFileName("basecl/baseglmodule.xml");

    registerProcessor(EntryExitPointsCL);
    registerProcessor(VolumeFirstHitCL);
}

BaseCLModule::~BaseCLModule()
{

}

} // namespace
             