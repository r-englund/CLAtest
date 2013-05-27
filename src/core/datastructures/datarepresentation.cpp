#include <inviwo/core/datastructures/datarepresentation.h>

namespace inviwo {

    DataRepresentation::DataRepresentation() : valid_(true), dataFormatBase_(DataUINT8()) {}
    DataRepresentation::DataRepresentation(DataFormatBase format) : valid_(true), dataFormatBase_(format) {}
    DataRepresentation::~DataRepresentation() {}

    std::string DataRepresentation::getClassName() const { 
        return "DataRepresentation"; 
    }

    bool DataRepresentation::isValid() const{
        return valid_;
    }

    void DataRepresentation::invalidate(){
        valid_ = false;
    }

    DataFormatBase DataRepresentation::getDataFormat() const { 
        return dataFormatBase_; 
    }
    
    std::string DataRepresentation::getDataFormatString() const { 
        return dataFormatBase_.getString(); 
    }

    DataFormatId DataRepresentation::getDataFormatId() const { 
        return dataFormatBase_.getId(); 
    }

} // namespace
