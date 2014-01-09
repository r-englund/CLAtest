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
 * Primary author : Peter Steneteg
 *
 **********************************************************************/

#include "exampleprocessor.h"
#include <inviwo/core/datastructures/geometry/simplemeshcreator.h>

namespace inviwo {

ProcessorClassName(ExampleProcessor, "ExampleProcessor"); 
ProcessorCategory(ExampleProcessor, "Geometry Creation");
ProcessorCodeState(ExampleProcessor, CODE_STATE_EXPERIMENTAL); 

ExampleProcessor::ExampleProcessor()
	: ProcessorGL()
	, inport_("volume.inport")
	, outport_("geometry.outport") {

	addPort(inport_);
	addPort(outport_);
}

ExampleProcessor::~ExampleProcessor() {}

void ExampleProcessor::initialize() {
	ProcessorGL::initialize();
}

void ExampleProcessor::deinitialize() {
	ProcessorGL::deinitialize();
}

void ExampleProcessor::process() {
}
} // namespace
