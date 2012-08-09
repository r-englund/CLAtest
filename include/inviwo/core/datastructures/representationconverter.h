#ifndef IVW_REPRESENTATIONCONVERTER_H
#define IVW_REPRESENTATIONCONVERTER_H

#include "inviwo/core/datastructures/datarepresentation.h"

namespace inviwo {

    class RepresentationConverter {

    public:
        RepresentationConverter();
        virtual ~RepresentationConverter();

        virtual bool canConvert(DataRepresentation* source, DataRepresentation* destination) = 0;
        virtual void convert(DataRepresentation* source, DataRepresentation* destination) = 0;
    };

} // namespace

#endif // IVW_REPRESENTATIONCONVERTER_H
