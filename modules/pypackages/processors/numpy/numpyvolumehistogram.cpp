/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
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
 * Contact: Sathish Kottravel
 *
 *********************************************************************************/

#include <modules/pypackages/processors/numpy/numpyvolumehistogram.h>
#include <inviwo/core/datastructures/volume/volumeram.h>
#include <inviwo/core/datastructures/volume/volumeramprecision.h>
#include <modules/pypackages/pypackagesmodule.h>

namespace inviwo {

ProcessorClassIdentifier(NumpyVolumeHistogram, "org.inviwo.NumpyVolumeHistogram");
ProcessorDisplayName(NumpyVolumeHistogram,  "Numpy Volume Histogram");
ProcessorTags(NumpyVolumeHistogram, "Python/CPU");
ProcessorCategory(NumpyVolumeHistogram, "Numpy");
ProcessorCodeState(NumpyVolumeHistogram, CODE_STATE_EXPERIMENTAL);

NumpyVolumeHistogram::NumpyVolumeHistogram()
    : PyProcessorBase()
    , inport_("inport")
    , outport_("outport")
    , savePDFDirectory_("pdfDir", "Output Directory", InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_IMAGES) ,
                         InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_IMAGES))
    , savePDFButton_("savePDF", "Save PDF") {

    addPort(inport_);
    addPort(outport_);

    addProperty(savePDFDirectory_);
    addProperty(savePDFButton_);
    //savePDFDirectory_.setGroupID("PDF Options");
    //savePDFButton_.setGroupID("PDF Options");

    pythonScriptFile_.set(InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_MODULES) +
                          "pypackages/scripts/numpy/numpyvolumehistogram.py");
    savePDFButton_.onChange(this, &NumpyVolumeHistogram::savePDF);
}

NumpyVolumeHistogram::~NumpyVolumeHistogram() {}

void NumpyVolumeHistogram::initialize() {
    PyProcessorBase::initialize();
    allocateBuffers();
}

void NumpyVolumeHistogram::deinitialize() {
    PyProcessorBase::deinitialize();
    deAllocateBuffers();
}

void NumpyVolumeHistogram::process() {
    Volume* volumeData = const_cast<Volume*>(inport_.getData());
    Image* image = outport_.getData();
    PyProcessorBase::addExistingVolume("SourceVolume", volumeData);
    PyProcessorBase::addExistingLayer("OutputImage", image->getColorLayer());
    PyProcessorBase::reloadScript();
    PyProcessorBase::process();
}

void NumpyVolumeHistogram::allocateBuffers() {
    PyProcessorBase::allocateLayer("OutputImage", DataVec4UINT8::str(), ivec2(256) );
    PyProcessorBase::allocatePyBuffer("SavePDF", DataUINT8::str(), 1);
}

void NumpyVolumeHistogram::deAllocateBuffers() {
    PyProcessorBase::freeAll();
}

void NumpyVolumeHistogram::savePDF() {
    PyProcessorBase::reloadScript();
    glm::uint8* savePDF = static_cast<glm::uint8*>(PyProcessorBase::getPyBufferData("SavePDF"));
    *savePDF = 1;
}

} // namespace
