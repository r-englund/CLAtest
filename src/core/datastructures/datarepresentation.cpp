#include <inviwo/core/datastructures/datarepresentation.h>

namespace inviwo {

DataRepresentation::DataRepresentation() 
    : dataFormatBase_(DataUINT8::get()) {}
DataRepresentation::DataRepresentation(const DataFormatBase* format) 
    : dataFormatBase_(format) {}
DataRepresentation::DataRepresentation(const DataRepresentation& rhs) 
    : dataFormatBase_(rhs.dataFormatBase_) {}
DataRepresentation::~DataRepresentation() { }

std::string DataRepresentation::getClassName() const { 
    return "DataRepresentation"; 
}

const DataFormatBase* DataRepresentation::getDataFormat() const { 
    return dataFormatBase_; 
}

std::string DataRepresentation::getDataFormatString() const { 
    return std::string(dataFormatBase_->getString()); 
}

DataFormatId DataRepresentation::getDataFormatId() const { 
    return dataFormatBase_->getId(); 
}

void DataRepresentation::setDataFormat(const DataFormatBase* format){
    dataFormatBase_ = format;
}

} // namespace
