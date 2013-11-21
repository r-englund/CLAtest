#include <inviwo/core/datastructures/data.h>

namespace inviwo {

Data::Data()
    : metaData_(new MetaDataMap())
    , dataFormatBase_(0)
    , validRepresentations_(0)
    , lastValidRepresentation_(NULL){ 
}

Data::Data(const Data& rhs) 
    : metaData_(rhs.metaData_->clone())
    , dataFormatBase_(rhs.dataFormatBase_){
    rhs.copyRepresentationsTo(this);
}

Data& Data::operator=(const Data& that){
    if (this != &that) {
        MetaDataMap* metadata = that.metaData_->clone();
        delete metaData_;
        metaData_ = metadata;

        that.copyRepresentationsTo(this);
        dataFormatBase_ = that.dataFormatBase_;
    }
    return *this;
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

void Data::copyRepresentationsTo(Data* targetData) const{
    targetData->clearRepresentations();
    int count = 0;
    for(int i=0; i<static_cast<int>(representations_.size()); i++) {
        DataRepresentation* rep = representations_[i]->clone();
        if(rep){
            targetData->addRepresentation(rep);
            if(isRepresentationValid(i)) 
                targetData->setRepresentationAsValid(count);
            if(representations_[i] == lastValidRepresentation_) 
                targetData->lastValidRepresentation_ = rep;
           
        }
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
            for(int i = static_cast<int>(representations_.size())-1; i >= 0; --i) {
                // Check if this representation is valid 
                // and make sure that it is not the one removed
                if(isRepresentationValid(i) && representations_[i] != representation) {
                    lastValidRepresentation_ = representations_[i];
                    break;
                }
            }
        }
        // Update valid representation bit mask
        size_t element = static_cast<size_t>(std::distance(representations_.begin(), it));
        // Start after the element that is going to be removed and update the mask.
        for(size_t i = element+1; i < representations_.size(); ++i) {
            if(isRepresentationValid(i)) {
                setRepresentationAsValid(i-1);
            } else {
                setRepresentationAsInvalid(i-1);
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
