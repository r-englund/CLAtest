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

#include "canvasprocessorgl.h"
#include "canvasgl.h"

namespace inviwo {

ProcessorClassName(CanvasProcessorGL, "CanvasGL"); 
ProcessorCategory(CanvasProcessorGL, "Data Output");
ProcessorCodeState(CanvasProcessorGL, CODE_STATE_STABLE);

CanvasProcessorGL::CanvasProcessorGL()
    : CanvasProcessor()
{}

void CanvasProcessorGL::initialize() {
    CanvasProcessor::initialize();
}

void CanvasProcessorGL::deinitialize() {
    CanvasProcessor::deinitialize();
}

void CanvasProcessorGL::process() {
    CanvasProcessor::process();
    static_cast<CanvasGL*>(getCanvas())->render(inport_.getData(), visibleLayer_.get());
}

} // namespace
