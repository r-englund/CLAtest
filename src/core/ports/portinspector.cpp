 /*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2014 Inviwo Foundation
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
 * Main file author: Peter Steneteg
 *
 *********************************************************************************/

#include <inviwo/core/ports/portinspector.h>
#include <inviwo/core/io/serialization/ivwserialization.h>

namespace inviwo {
PortInspector::PortInspector(std::string portClassName, std::string inspectorWorkspace)
    : inspectorNetworkFileName_(inspectorWorkspace)
    , portClassName_(portClassName)
    , active_(false)
    , inspectorNetwork_(NULL) {
    
    InviwoApplication::getPtr()->registerFileObserver(this);
}

PortInspector::~PortInspector() {
    if (inspectorNetwork_)
        delete inspectorNetwork_;
}

void PortInspector::setActive(bool val) {
    active_ = val;
}

bool PortInspector::isActive() {
    return active_;
}

std::string PortInspector::getInspectorNetworkFileName() {
    return inspectorNetworkFileName_;
}

std::string PortInspector::getPortClassName() {
    return portClassName_;
}

std::vector<Inport*> PortInspector::getInports() {
    if (!inspectorNetwork_)
        initialize();

    return inPorts_;
}

CanvasProcessor* PortInspector::getCanvasProcessor() {
    if (!inspectorNetwork_)
        initialize();

    return canvasProcessor_;
}

std::vector<PortConnection*>  PortInspector::getConnections() {
    if (!inspectorNetwork_)
        initialize();

    return connections_;
}
std::vector<ProcessorLink*> PortInspector::getProcessorLinks() {
    if (!inspectorNetwork_)
        initialize();

    return processorLinks_;
}
std::vector<Processor*> PortInspector::getProcessors() {
    if (!inspectorNetwork_)
        initialize();

    return processors_;
}

void PortInspector::fileChanged(std::string fileName){
    if (inspectorNetwork_)
        delete inspectorNetwork_;
    initialize();
}

void PortInspector::initialize() {
    //Observe the filename;
    startFileObservation(inspectorNetworkFileName_);
    
    // Deserialize the network
    IvwDeserializer xmlDeserializer(inspectorNetworkFileName_);
    inspectorNetwork_ = new ProcessorNetwork();
    inspectorNetwork_->deserialize(xmlDeserializer);
    processors_ = inspectorNetwork_->getProcessors();

    for (size_t i = 0; i < processors_.size(); i++) {
        Processor* processor = processors_[i];
        // Set Identifiers
        std::string newIdentifier = getPortClassName()+"_Port_Inspector_"+processor->getIdentifier();
        processor->setIdentifier(newIdentifier);
        processor->initialize();
        // Find the and save inports.
        std::vector<Inport*> inports = processor->getInports();

        for (size_t i = 0; i < inports.size(); i++) {
            if (!inports[i]->isConnected())
                inPorts_.push_back(inports[i]);
        }

        // Find and save the canvasProcessor
        CanvasProcessor* canvasProcessor = dynamic_cast<CanvasProcessor*>(processor);

        if (canvasProcessor)
            canvasProcessor_ = canvasProcessor;

    }

    // Store the connections and and disconnect them.
    connections_ = inspectorNetwork_->getConnections();

    for (size_t i=0; i<connections_.size(); i++)
        connections_[i]->getInport()->disconnectFrom(connections_[i]->getOutport());

    // store the processor links.
    processorLinks_ = inspectorNetwork_->getLinks();
}





} // namespace

