#include <inviwo/core/datastructures/data.h>

namespace inviwo {

Data::Data(): validRepresentations_(0), lastValidRepresentation_(NULL) {}
Data::~Data() { clearRepresentations(); }

void Data::clearRepresentations() {
    setAllRepresentationsAsInvalid();
    while (hasRepresentations()) {
        delete representations_.back();
        representations_.pop_back();
    }
}

void Data::copyRepresentations( Data* targetData ) const{
    targetData->representations_.clear();
    for(size_t i=0; i<representations_.size(); i++) {
        targetData->representations_.push_back(representations_[i]->clone());
    }
}

void Data::addRepresentation(DataRepresentation* representation) {
    representations_.push_back(representation);
    lastValidRepresentation_ = representation;
    setRepresentationAsValid(static_cast<int>(representations_.size())-1);
}

bool Data::hasRepresentations() const {
    return !representations_.empty();
}

void Data::setDataFormat(DataFormatBase format){
    dataFormatBase_ = format;
}

DataFormatBase Data::getDataFormat() const{
    return dataFormatBase_;
}

} // namespace
