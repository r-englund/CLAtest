#include <inviwo/core/datastructures/datagrouprepresentation.h>

namespace inviwo {

DataGroupRepresentation::DataGroupRepresentation(std::vector<Data*>* data){
    data_ = data;
}

DataGroupRepresentation::DataGroupRepresentation(const DataGroupRepresentation& rhs) {
    data_ = rhs.data_;
}

DataGroupRepresentation::~DataGroupRepresentation() { }

std::string DataGroupRepresentation::getClassName() const { 
    return "DataGroupRepresentation"; 
}

} // namespace
