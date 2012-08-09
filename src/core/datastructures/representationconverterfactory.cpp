#include "inviwo/core/datastructures/representationconverterfactory.h"

namespace inviwo {

    RepresentationConverterFactory::RepresentationConverterFactory() {}
    RepresentationConverterFactory::~RepresentationConverterFactory() {}

    void RepresentationConverterFactory::registerRepresentationConverter(RepresentationConverter* representationConverter) {
        representationConverters_.push_back(representationConverter);
    }

    RepresentationConverter* RepresentationConverterFactory::getRepresentationConverter(DataRepresentation* source, DataRepresentation* destination) {
        // TODO: optimize performance, e.g., by using a hash table
        for (unsigned int i=0; i<representationConverters_.size(); i++) {
            if (representationConverters_[i]->canConvert(source, destination))
                return representationConverters_[i];
        }
        return 0; //TODO: throw an exception here
    }

} // namespace
