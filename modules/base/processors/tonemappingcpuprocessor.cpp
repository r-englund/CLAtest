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
 * Main file authors: Erik Sundén
 *
 *********************************************************************************/

#include "tonemappingcpuprocessor.h"
#include <inviwo/core/io/imageio.h>

namespace inviwo {

ProcessorClassName(ToneMappingCPUProcessor, "ToneMappingCPUProcessor");
ProcessorCategory(ToneMappingCPUProcessor, "Image Processing");
ProcessorCodeState(ToneMappingCPUProcessor, CODE_STATE_EXPERIMENTAL);

ToneMappingCPUProcessor::ToneMappingCPUProcessor()
    : Processor(),
      inport_("image.inport"),
      outport_("image.outport"),
      toneMappingMethod_("toneMappingMethod", "Tone Mapping Method")
{
    addPort(inport_);
    addPort(outport_);

    toneMappingMethod_.addOption("DRAGO03",          "Adaptive logarithmic mapping",     "FITMO_DRAGO03");
    toneMappingMethod_.addOption("REINHARD05",       "Dynamic range reduction",          "FITMO_REINHARD05");
    toneMappingMethod_.addOption("FATTAL02",         "Gradient domain compression",      "FITMO_FATTAL02");
    toneMappingMethod_.addOption("PASS",             "Pass-through",                     "PASS");
    toneMappingMethod_.setSelectedIndex(0);
    toneMappingMethod_.onChange(this, &ToneMappingCPUProcessor::toneMappingMethodChanged);
    addProperty(toneMappingMethod_);
}

ToneMappingCPUProcessor::~ToneMappingCPUProcessor() {}

void ToneMappingCPUProcessor::initialize() {
    Processor::initialize();
    toneMappingMethodChanged();
}

void ToneMappingCPUProcessor::deinitialize() {
    Processor::deinitialize();
}

void ToneMappingCPUProcessor::process() {
    if(passSelected_){
        outport_.setConstData(inport_.getData());
        return;
    }

    const LayerRAM* imageRam = inport_.getData()->getColorLayer()->getRepresentation<LayerRAM>();
    if(imageRam){
        FREE_IMAGE_TMO tmo;
        double first = 0.0;
        double second = 0.0;
        if(drago03Selected_){
            tmo = FITMO_DRAGO03;
        }
        else if(drago03Selected_){
            tmo = FITMO_REINHARD05;
        }
        else{
            tmo = FITMO_FATTAL02;
        }

        //ImageIO::initLoader();
        //FIBITMAP* bitmap = ImageIO::createBitmapFromData(imageRam);
        //FIBITMAP* result = FreeImage_ToneMapping(bitmap, tmo, first, second);
        //FreeImage_Unload(bitmap);
    }
}

void ToneMappingCPUProcessor::toneMappingMethodChanged() {
    drago03Selected_ = (toneMappingMethod_.getSelectedIdentifier() == "DRAGO03");
    reinhard05Selected_ = (toneMappingMethod_.getSelectedIdentifier() == "REINHARD05");
    fattal02Selected_ = (toneMappingMethod_.getSelectedIdentifier() == "FATTAL02");
    passSelected_ = (toneMappingMethod_.getSelectedIdentifier() == "PASS");
}

} // namespace
