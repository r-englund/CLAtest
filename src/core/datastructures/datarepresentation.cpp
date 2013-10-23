#include <inviwo/core/datastructures/datarepresentation.h>

namespace inviwo {

DataRepresentation::DataRepresentation() : valid_(true), dataFormatBase_(DataUINT8::get()) {}
DataRepresentation::DataRepresentation(const DataFormatBase* format) : valid_(true), dataFormatBase_(format) {}
DataRepresentation::~DataRepresentation() { }

std::string DataRepresentation::getClassName() const { 
    return "DataRepresentation"; 
}

bool DataRepresentation::isValid() const{
    return valid_;
}

void DataRepresentation::invalidate(){
    valid_ = false;
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
