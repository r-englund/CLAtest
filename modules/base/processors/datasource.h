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
class DataSource : public Processor {
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
    DataType* loadedData_;
    bool isDeserializing_;
};

template <typename DataType, typename PortType>
DataSource<DataType, PortType>::DataSource()
    : Processor(), port_("data"), file_("filename", "File"), loadedData_(NULL), isDeserializing_(false) {
    addPort(port_);
    file_.onChange(this, &DataSource::load);
    std::vector<FileExtension> ext = DataReaderFactory::getPtr()->getExtensionsForType<DataType>();

    for (std::vector<FileExtension>::const_iterator it = ext.begin(); it != ext.end(); ++it) {
        std::stringstream ss;
        ss << it->description_ << " (*." << it->extension_ << ")";
        file_.addNameFilter(ss.str());
    }

    addProperty(file_);
}

template <typename DataType, typename PortType>
DataSource<DataType, PortType>::~DataSource() {}

template <typename DataType, typename PortType>
void DataSource<DataType, PortType>::invalidateOutput() {
    invalidate(PropertyOwner::INVALID_OUTPUT);
}

template <typename DataType, typename PortType>
bool DataSource<DataType, PortType>::isReady() const {
    return URLParser::fileExists(file_.get());
}

/**
 * load is called when the filename changes, and after the deserialisation
 */
template <typename DataType, typename PortType>
void DataSource<DataType, PortType>::load() {
    if (isDeserializing_ || file_.get() == "") {
        return;
    }
    TemplateResource<DataType>* resource =
        ResourceManager::getPtr()->getResourceAs<TemplateResource<DataType> >(file_.get());

    if (resource) {
        port_.setData(resource->getData(), false);
        loadedData_ = resource->getData();
        dataLoaded(resource->getData());
    } else {
        std::string fileExtension = URLParser::getFileExtension(file_.get());
        DataReaderType<DataType>* reader =
            DataReaderFactory::getPtr()->getReaderForTypeAndExtension<DataType>(fileExtension);

        if (reader) {
            try {
                DataType* data = reader->readMetaData(file_.get());
                ResourceManager::getPtr()->addResource(
                    new TemplateResource<DataType>(file_.get(), data));
                port_.setData(data, false);
                loadedData_ = data;
                dataLoaded(data);
            }
            catch (DataReaderException const& e) {
                LogError("Could not load data: " << file_.get() << ", " << e.getMessage());
                file_.set("");
            }
        } else {
            LogError("Could not find a data reader for file: " << file_.get());
            file_.set("");
        }
    }
}

template <typename DataType, typename PortType>
void inviwo::DataSource<DataType, PortType>::serialize(IvwSerializer& s) const {
    Processor::serialize(s);
}
/**
 * Deserialize everything first then load the data
 */
template <typename DataType, typename PortType>
void inviwo::DataSource<DataType, PortType>::deserialize(IvwDeserializer& d) {
    isDeserializing_ = true;
    Processor::deserialize(d);
    isDeserializing_ = false;
    load();
}

}  // namespace

#endif  // IVW_DATASOURCE_H
