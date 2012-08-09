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

} // namespace
