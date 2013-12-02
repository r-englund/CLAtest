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

#include "redgreenprocessor.h"

namespace inviwo {

ProcessorClassName(RedGreenProcessor, "RedGreenProcessor"); 
ProcessorCategory(RedGreenProcessor, "Various");
ProcessorCodeState(RedGreenProcessor, CODE_STATE_STABLE);

RedGreenProcessor::RedGreenProcessor()
    : ProcessorGL(),
    outportRed_("outportRed"),
    outportGreen_("outportGreen")
{
    addPort(outportRed_);
    addPort(outportGreen_);
}

RedGreenProcessor::~RedGreenProcessor() {}

void RedGreenProcessor::initialize() {
    Processor::initialize();
}

void RedGreenProcessor::deinitialize() {
    Processor::deinitialize();
}

void RedGreenProcessor::process() {
    activateTarget(outportRed_);
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    deactivateCurrentTarget();

    activateTarget(outportGreen_);
    glClearColor(0.0, 1.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    deactivateCurrentTarget();
}

} // namespace
