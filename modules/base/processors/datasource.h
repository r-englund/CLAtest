#ifndef IVW_DATASOURCE_H
#define IVW_DATASOURCE_H

#include <modules/base/basemoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/properties/fileproperty.h>
#include <inviwo/core/io/datareaderfactory.h>
#include <inviwo/core/util/urlparser.h>
#include <inviwo/core/resources/resourcemanager.h>
#include <inviwo/core/resources/templateresource.h>

namespace inviwo {

template <typename DataType, typename PortType>
class IVW_MODULE_BASE_API DataSource : public Processor {
public:
    DataSource();
    virtual ~DataSource();

    virtual bool isReady() const;

protected:
    void load();

    virtual void dataLoaded(DataType* data) {};

    virtual void invalidateOutput();

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

    PortType port_;
    FileProperty file_;
    bool isDeserializing_;
};

template <typename DataType, typename PortType>
DataSource<DataType, PortType>::DataSource()
    : Processor()
    , port_("data")
    , file_("filename", "File")
    , isDeserializing_(false) {
    addPort(port_);
    file_.onChange(this, &DataSource::load);
    std::vector<FileExtension> ext = DataReaderFactory::getRef().getExtensionsForType<DataType>();

    for (std::vector<FileExtension>::const_iterator it = ext.begin();
         it != ext.end(); ++it) {
        std::stringstream ss;
        ss << it->description_ << " (*." << it->extension_ << ")";
        file_.addNameFilter(ss.str());
    }

    addProperty(file_);
}

template <typename DataType, typename PortType>
DataSource<DataType, PortType>::~DataSource() {
}

template <typename DataType, typename PortType>
void DataSource<DataType, PortType>::invalidateOutput() {
    port_.invalidate(PropertyOwner::INVALID_OUTPUT);
}

template <typename DataType, typename PortType>
bool DataSource<DataType, PortType>::isReady() const {
    return URLParser::fileExists(file_.get());
}

template <typename DataType, typename PortType>
void DataSource<DataType, PortType>::load() {
    if (isDeserializing_) {
        return;
    }
    TemplateResource<DataType>* resource = ResourceManager::getPtr()->getResourceAs<TemplateResource<DataType> >(file_.get());

    if (resource) {
        port_.setData(resource->getData(), false);
        dataLoaded(resource->getData());
    } else {
        std::string fileExtension = URLParser::getFileExtension(file_.get());
        DataReaderType<DataType>* reader = DataReaderFactory::getRef().getReaderForTypeAndExtension<DataType>(fileExtension);

        if (reader) {
            try {
                DataType* data = reader->readMetaData(file_.get());
                ResourceManager::getPtr()->addResource(new TemplateResource<DataType>(file_.get(), data));
                port_.setData(data, false);
                dataLoaded(data);
            } catch (DataReaderException const& e) {
                LogError(e.getMessage());
            }
        } else {
            LogError("Could not load data: " << file_.get());
        }
    }
}

template <typename DataType, typename PortType>
void inviwo::DataSource<DataType, PortType>::serialize(IvwSerializer& s) const {
    Processor::serialize(s);
}
template <typename DataType, typename PortType>
void inviwo::DataSource<DataType, PortType>::deserialize(IvwDeserializer& d) {
    isDeserializing_ = true;
    Processor::deserialize(d);
    isDeserializing_ = false;
    load();
}


} // namespace

#endif // IVW_DATASOURCE_H

