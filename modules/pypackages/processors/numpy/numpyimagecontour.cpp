/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.9
 *
 * Copyright (c) 2014-2015 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 *********************************************************************************/

#include <modules/pypackages/processors/numpy/numpyimagecontour.h>
#include <inviwo/core/datastructures/image/imageram.h>
#include <inviwo/core/datastructures/image/imageramprecision.h>
#include <modules/pypackages/pypackagesmodule.h>

namespace inviwo {

ProcessorClassIdentifier(NumpyImageContour, "org.inviwo.NumpyImageContour");
ProcessorDisplayName(NumpyImageContour,  "Numpy Image Contour");
ProcessorTags(NumpyImageContour, "Python/CPU");
ProcessorCategory(NumpyImageContour, "Numpy");
ProcessorCodeState(NumpyImageContour, CODE_STATE_EXPERIMENTAL);

NumpyImageContour::NumpyImageContour()
    : PyProcessorBase()
    , inport_("inport")
    , outport_("outport")
    , contourValues_("contourvalues", "ContourValues", 64, 192, 0, 255, 1, 1)
    , instantUpdate_("instantUpdate", "Update Instantly", false) {
    addPort(inport_);
    addPort(outport_);
    addProperty(instantUpdate_);
    addProperty(contourValues_);
    pythonScriptFile_.set(InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_MODULES) +
                          "pypackages/scripts/numpy/numpyimagecontour.py");
}

NumpyImageContour::~NumpyImageContour() {}

void NumpyImageContour::initialize() {
    PyProcessorBase::initialize();
    allocateBuffers();
}

void NumpyImageContour::deinitialize() {
    PyProcessorBase::deinitialize();
    deAllocateBuffers();
}

void NumpyImageContour::process() {
    Image* imageData = inport_.getData()->clone();
    PyProcessorBase::addExistingLayer("SourceImage", imageData->getColorLayer());
    if (instantUpdate_.get())
        reloadScript();    
    PyProcessorBase::process();
    outport_.setData(imageData);
}

void NumpyImageContour::allocateBuffers() {
    PyProcessorBase::allocateLayer("SourceImage", DataVec4UINT8::str(), ivec2(256) );
}

void NumpyImageContour::deAllocateBuffers() {
    PyProcessorBase::freeAll();
}

} // namespace
