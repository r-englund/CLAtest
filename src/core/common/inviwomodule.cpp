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
 * Primary author : Peter Steneteg
 *
 **********************************************************************/


#include <inviwo/core/common/inviwomodule.h>
#include <inviwo/core/common/inviwoapplication.h>

namespace inviwo {

InviwoModule::InviwoModule()
    : identifier_("undefined")
    , initialized_(false)
    , xmlDocumentFileName_("undefined")
{}

InviwoModule::~InviwoModule() {
    if (isInitialized())
        LogWarn("Module '" + getIdentifier() + "' should have been deinitialized before destruction.");

    for (size_t i=0; i<capabilities_.size(); i++)
        delete capabilities_[i];
    capabilities_.clear();

    for (size_t i=0; i<data_.size(); i++)
        delete data_[i];
    data_.clear();

    for (size_t i=0; i<dataReaders_.size(); i++)
        delete dataReaders_[i];
    dataReaders_.clear();

    for (size_t i=0; i<dataRepresentations_.size(); i++)
        delete dataRepresentations_[i];
    dataRepresentations_.clear();

    for (size_t i=0; i<dataWriters_.size(); i++)
        delete dataWriters_[i];
    dataWriters_.clear();

    for (size_t i=0; i<metadata_.size(); i++)
        delete metadata_[i];
    metadata_.clear();

    for (size_t i=0; i<ports_.size(); i++)
        delete ports_[i];
    ports_.clear();

    for(size_t i = 0; i < portInspectors_.size(); i++)
        delete portInspectors_[i];
    portInspectors_.clear();

    for (size_t i=0; i<processors_.size(); i++)
        delete processors_[i];
    processors_.clear();

    for (size_t i=0; i<processorWidgets_.size(); i++)
        delete processorWidgets_[i].second;
    processorWidgets_.clear();

    for (size_t i=0; i<properties_.size(); i++)
        delete properties_[i];
    properties_.clear();

    for (size_t i=0; i<representationConverters_.size(); i++)
        delete representationConverters_[i];
    representationConverters_.clear();

    for (size_t i=0; i<resources_.size(); i++)
        delete resources_[i];
    resources_.clear();

    for (size_t i=0; i<renderers_.size(); i++)
        delete renderers_[i];
    renderers_.clear();



    for (size_t i=0; i<moduleSettings_.size(); i++)
        delete moduleSettings_[i];
    moduleSettings_.clear();
}

std::string InviwoModule::getIdentifier() const {
    return identifier_;
}

std::string InviwoModule::getPath(const std::string& suffix) const {
    return InviwoApplication::getRef().getPath(InviwoApplication::PATH_MODULES, suffix);
}

void InviwoModule::initialize() {
    for(size_t i=0; i<capabilities_.size(); i++){
        capabilities_[i]->initialize();
        capabilities_[i]->printInfo();
    }

    setupModuleSettings();

    initialized_ = true;
}

bool InviwoModule::isInitialized() const {
    return initialized_;
}

void InviwoModule::deinitialize() {
    initialized_ = false;
}

void InviwoModule::setIdentifier(const std::string& identifier) {
    identifier_ = identifier;
    if (xmlDocumentFileName_ == "undefined")
        xmlDocumentFileName_ = identifier_ + ".xml";
}

void InviwoModule::setXMLFileName(const std::string& xmlFileName) {
    xmlDocumentFileName_ = getPath() + "/" + xmlFileName;
}

const std::vector<Capabilities*>& InviwoModule::getCapabilities() const {return capabilities_;}
const std::vector<Data*>& InviwoModule::getData() const {return data_;}
const std::vector<DataReader*>& InviwoModule::getDataReaders() const {return dataReaders_;}
const std::vector<DataRepresentation*>& InviwoModule::getDataRepresentations() const {return dataRepresentations_;}
const std::vector<DataWriter*>& InviwoModule::getDataWriters() const {return dataWriters_;}
const std::vector<MetaData*>& InviwoModule::getMetaData() const {return metadata_;}
const std::vector<Port*>& InviwoModule::getPorts() const {return ports_;}
const std::vector<PortInspector*>& InviwoModule::getPortInspectors() const { return portInspectors_; }
const std::vector<ProcessorFactoryObject*>& InviwoModule::getProcessors() const {return processors_;}
const std::vector< std::pair<std::string, ProcessorWidget*> >& InviwoModule::getProcessorWidgets() const {return processorWidgets_;}
const std::vector<Property*>& InviwoModule::getProperties() const {return properties_;}
const std::vector<PropertyWidgetFactoryObject*>& InviwoModule::getPropertyWidgets() const {return propertyWidgets_; }
const std::vector<RepresentationConverter*>& InviwoModule::getRepresentationConverters() const {return representationConverters_;}
const std::vector<GeometryRenderer*>& InviwoModule::getRenderers() const { return renderers_; }
const std::vector<Resource*>& InviwoModule::getResources() const {return resources_;}
const std::vector<Settings*>& InviwoModule::getSettings() const {return moduleSettings_;}

void InviwoModule::registerCapabilities(Capabilities* info) {capabilities_.push_back(info);}
void InviwoModule::registerData(Data* data) {data_.push_back(data);}
void InviwoModule::registerDataReader(DataReader* dataReader) {dataReaders_.push_back(dataReader);}
void InviwoModule::registerDataRepresentation(DataRepresentation* dataRepresentation) {dataRepresentations_.push_back(dataRepresentation);}
void InviwoModule::registerDataWriter(DataWriter* dataWriter) {dataWriters_.push_back(dataWriter);}
void InviwoModule::registerMetaData(MetaData* meta) {metadata_.push_back(meta);}
void InviwoModule::registerPort(Port* port) {ports_.push_back(port);}
void InviwoModule::registerPortInspector(PortInspector* portInspector) { portInspectors_.push_back(portInspector); }
void InviwoModule::registerProcessorObject(ProcessorFactoryObject* processor) { processors_.push_back(processor); }
void InviwoModule::registerProcessorWidget(std::string processorClassName, ProcessorWidget* processorWidget) { processorWidgets_.push_back(std::make_pair(processorClassName, processorWidget));}
void InviwoModule::registerProperty(Property* property) {properties_.push_back(property);}
void InviwoModule::registerPropertyWidgetObject(PropertyWidgetFactoryObject* propertyWidget) { propertyWidgets_.push_back(propertyWidget); }
void InviwoModule::registerRenderer(GeometryRenderer* renderer) { renderers_.push_back(renderer); }
void InviwoModule::registerRepresentationConverter(RepresentationConverter* representationConverter) {representationConverters_.push_back(representationConverter);}
void InviwoModule::registerResource(Resource* resource) {resources_.push_back(resource);}
void InviwoModule::registerSettings(Settings* settings) {moduleSettings_.push_back(settings);}

} // namespace