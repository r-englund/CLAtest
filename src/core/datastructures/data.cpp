#include <inviwo/core/datastructures/data.h>

namespace inviwo {

Data::Data(): validRepresentations_(0), lastValidRepresentation_(NULL) {
    metaData_ = new MetaDataMap();
}
Data::~Data() {
    delete metaData_;
    clearRepresentations();
}

void Data::clearRepresentations() {
    setAllRepresentationsAsInvalid();
    while (hasRepresentations()) {
        delete representations_.back();
        representations_.pop_back();
    }
}

void Data::copyRepresentations(Data* targetData) const{
    targetData->clearRepresentations();
    for(size_t i=0; i<representations_.size(); i++) {
        DataRepresentation* rep = representations_[i]->clone();
        if(rep)
            targetData->addRepresentation(rep);
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

void Data::setDataFormat(const DataFormatBase* format){
    dataFormatBase_ = format;
}

const DataFormatBase* Data::getDataFormat() const{
    return dataFormatBase_;
}

} // namespace
