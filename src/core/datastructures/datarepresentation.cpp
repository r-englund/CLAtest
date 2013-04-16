#include <inviwo/core/datastructures/datarepresentation.h>

namespace inviwo {

    DataRepresentation::DataRepresentation() : valid_(true) {}
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

} // namespace
