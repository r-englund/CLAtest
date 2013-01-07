#include "inviwo/core/datastructures/data.h"

namespace inviwo {

    Data::Data() {}
    Data::~Data() {}

    void Data::clearRepresentations() {
        while (!representations_.empty()) {
            delete representations_.back();
            representations_.pop_back();
        }
    }

    void Data::copyRepresentations(Data* targetData) {
        targetData->representations_.clear();
        for(size_t i=0; i<representations_.size(); i++) {
            targetData->representations_.push_back(representations_[i]->clone());
        }
    }

    void Data::addRepresentation(DataRepresentation* representation) {
        representations_.push_back(representation);
    }

} // namespace
