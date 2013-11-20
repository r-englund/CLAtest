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
    newRepresentationCreated();
}

void Data::removeRepresentation( DataRepresentation* representation )
{
    std::vector<DataRepresentation*>::iterator it = std::find(representations_.begin(), representations_.end(), representation);
    if(it != representations_.end()) {
        // Update last valid representation
        if(lastValidRepresentation_ == *it) {
            lastValidRepresentation_ = NULL;
            for(size_t i = representations_.size()-1; i >= 0; --i) {
                // Check if this representation is valid 
                // and make sure that it is not the one removed
                if(isRepresentationValid(i) && representations_[i] != representation) {
                    lastValidRepresentation_ = representations_[i];
                    break;
                }
            }
        }
        delete (*it);
        representations_.erase(it);
    }
    
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
