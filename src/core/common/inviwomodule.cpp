#include <inviwo/core/common/inviwomodule.h>
#include <inviwo/core/common/inviwoapplication.h>

namespace inviwo {

InviwoModule::InviwoModule()
    : identifier_("undefined")
    , initialized_(false)
    , applicationSettings_(NULL)
    , xmlDocuFileName_("undefined")
{}

InviwoModule::~InviwoModule() {
    if (isInitialized())
        LogWarn("Module '" + getIdentifier() + "' should have been deinitialized before destruction.");

    for (size_t i=0; i<processors_.size(); i++)
        delete processors_[i];
    processors_.clear();

    for (size_t i=0; i<processorWidgets_.size(); i++)
        delete processorWidgets_[i].second;
    processorWidgets_.clear();

    for (size_t i=0; i<properties_.size(); i++)
        delete properties_[i];
    properties_.clear();

    for (size_t i=0; i<dataReaders_.size(); i++)
        delete dataReaders_[i];
    dataReaders_.clear();

    for (size_t i=0; i<dataWriters_.size(); i++)
        delete dataWriters_[i];
    dataWriters_.clear();

    for (size_t i=0; i<resourceInfos_.size(); i++)
        delete resourceInfos_[i];
    resourceInfos_.clear();
}

std::string InviwoModule::getIdentifier() const {
    return identifier_;
}

std::string InviwoModule::getPath(const std::string& suffix) const {
    return InviwoApplication::getRef().getPath(InviwoApplication::PATH_MODULES, suffix);
}

const std::vector<ProcessorFactoryObject*>& InviwoModule::getProcessors() const {
    return processors_;
}

const std::vector<std::pair<std::string, ProcessorWidget*>>& InviwoModule::getProcessorWidgets() const {
    return processorWidgets_;
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

const std::vector<MetaData*>& InviwoModule::getMetaData() const {
    return metadata_;
}

const std::vector<RepresentationConverter*>& InviwoModule::getRepresentationConverters() const {
    return representationConverters_;
}

std::vector<ResourceInfo*>& InviwoModule::getResourceInfos() {
    return resourceInfos_;
}

void InviwoModule::setGlobalSettings(Settings* settings){
    if (settings){
        applicationSettings_ = settings;
        setupModuleSettings();
    }
}

void InviwoModule::initialize() {
    for(size_t i=0; i<resourceInfos_.size(); i++){
        resourceInfos_[i]->initialize();
        resourceInfos_[i]->printInfo();
    }

    initialized_ = true;
}

void InviwoModule::deinitialize() {
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

void InviwoModule::addProcessor(ProcessorFactoryObject* processor) {
    // FIXME: check processor pointer and module's name
    //processor->setModuleIdentifier(identifier_);
    processors_.push_back(processor);
}

void InviwoModule::addProcessorWidget(std::string processorClassName, ProcessorWidget* processorWidget) {
    processorWidgets_.push_back(std::make_pair(processorClassName, processorWidget));
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

void InviwoModule::addResourceInfo(ResourceInfo* resourceInfo) {
    resourceInfos_.push_back(resourceInfo);
}

bool InviwoModule::isInitialized() const {
    return initialized_;
}

} // namespace
