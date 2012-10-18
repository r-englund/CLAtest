#include "inviwo/core/inviwomodule.h"
#include "inviwo/core/inviwoapplication.h"

namespace inviwo {

const std::string InviwoModule::logSource_ = "InviwoModule";

InviwoModule::InviwoModule()
    : identifier_("undefined")
    , xmlDocuFileName_("undefined")
    , initialized_(false)
{}

InviwoModule::~InviwoModule() {
    if (isInitialized())
        LogWarn("Module '" + getIdentifier() + "' should have been deinitialized before destruction.");

    for (size_t i=0; i<processors_.size(); i++)
        delete processors_[i];
    processors_.clear();

    for (size_t i=0; i<properties_.size(); i++)
        delete properties_[i];
    properties_.clear();

    for (size_t i=0; i<dataReaders_.size(); i++)
        delete dataReaders_[i];
    dataReaders_.clear();

    for (size_t i=0; i<dataWriters_.size(); i++)
        delete dataWriters_[i];
    dataWriters_.clear();
}

std::string InviwoModule::getIdentifier() const {
    return identifier_;
}

std::string InviwoModule::getPath(const std::string& suffix) const {
    return InviwoApplication::app()->getPath(InviwoApplication::PATH_MODULES, suffix);
}

const std::vector<Processor*>& InviwoModule::getProcessors() const {
    return processors_;
}

const std::vector<Property*>& InviwoModule::getProperties() const {
    return properties_;
}

const std::vector<DataReader*>& InviwoModule::getDataReaders() const {
    return dataReaders_;
}

const std::vector<DataWriter*>& InviwoModule::getDataWriters() const {
    return dataWriters_;
}

const std::vector<RepresentationConverter*>& InviwoModule::getRepresentationConverters() const {
    return representationConverters_;
}

const std::vector<MetaData*>& InviwoModule::getMetaData() const {
    return metadata_;
}

void InviwoModule::initialize() throw (Exception) {
    initialized_ = true;
}

void InviwoModule::deinitialize() throw (Exception) {
    initialized_ = false;
}

void InviwoModule::setIdentifier(const std::string& identifier) {
    identifier_ = identifier;
    if (xmlDocuFileName_ == "undefined")
        xmlDocuFileName_ = identifier_ + ".xml";
}

void InviwoModule::setXMLFileName(const std::string& xmlFileName) {
    xmlDocuFileName_ = getPath() + "/" + xmlFileName;
}

void InviwoModule::addProcessor(Processor* processor) {
    // check processor pointer and module's name
    //processor->setModuleIdentifier(identifier_);
    processors_.push_back(processor);
}

void InviwoModule::addMetaData(MetaData* meta) {
    metadata_.push_back(meta);
}

void InviwoModule::addProperty(Property* property) {
    properties_.push_back(property);
}

void InviwoModule::addDataReader(DataReader* dataReader) {
    dataReaders_.push_back(dataReader);
}

void InviwoModule::addDataWriter(DataWriter* dataWriter) {
    dataWriters_.push_back(dataWriter);
}

void InviwoModule::addRepresentationConverter(RepresentationConverter* representationConverter) {
    representationConverters_.push_back(representationConverter);
}

bool InviwoModule::isInitialized() const {
    return initialized_;
}

} // namespace
