#include <inviwo/core/datastructures/datagroup.h>

namespace inviwo {

DataGroup::DataGroup(){ 
}

DataGroup::DataGroup(const DataGroup& rhs){
    //TOD0: Implement
}

DataGroup& DataGroup::operator=(const DataGroup& that){
    if (this != &that) {
        //TOD0: Implement 
    }
    return *this;
}


DataGroup::~DataGroup() {
}

void DataGroup::addData(Data* dataObj){
    data_.push_back(dataObj);
}

void DataGroup::clearRepresentations() {
    while (hasRepresentations()) {
        delete representations_.back();
        representations_.pop_back();
    }
}

bool DataGroup::hasRepresentations() const {
    return !representations_.empty();
}

} // namespace
