/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013 Inviwo Foundation
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
 * Main file author: Riccardo Volpi
 *
 *********************************************************************************/

#include "gromacsstaticaddinfoprocessor.h"

#include <inviwo/core/resources/resourcemanager.h>
#include <inviwo/core/resources/templateresource.h>
#include <inviwo/core/io/datareaderfactory.h>
#include <modules/gromacs/gromacsstaticaddinforeader.h>

namespace inviwo {
    
    ProcessorClassName(GromacsStaticAddInfoProcessor, "Gromacs Static Additional Info");
    ProcessorCategory(GromacsStaticAddInfoProcessor, "Data Input");
    ProcessorCodeState(GromacsStaticAddInfoProcessor, CODE_STATE_EXPERIMENTAL);
    
GromacsStaticAddInfoProcessor::GromacsStaticAddInfoProcessor()
    : Processor(),
    outPort_("gromacs"),
    fileProp_("fileName", "Gromacs static info file") {

    addPort(outPort_);

    fileProp_.onChange(this, &GromacsStaticAddInfoProcessor::load);
    fileProp_.addNameFilter("Static Additional Gromacs Info (*.sag)");

    addProperty(fileProp_);
}
    
GromacsStaticAddInfoProcessor::~GromacsStaticAddInfoProcessor() {}
    
void GromacsStaticAddInfoProcessor::load() {
    if(fileProp_.get().empty()) {
            return;
    }
    if(URLParser::fileExists(fileProp_.get())) {
        AddInfoReader reader;
        GromacsTimeStep *data = new GromacsTimeStep();
        if(reader.read(fileProp_.get(), data)) {
            outPort_.setData(data);
        } else {
            delete data;
            LogInfo("Failed to read file: " + fileProp_.get());
        }  
    } else {
        LogInfo("File does not exist: " + fileProp_.get());
    }
    
}
    
} // namespace
