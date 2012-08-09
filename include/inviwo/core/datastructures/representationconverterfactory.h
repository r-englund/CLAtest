#ifndef IVW_REPRESENTATIONCONVERTERFACTORY_H
#define IVW_REPRESENTATIONCONVERTERFACTORY_H

#include "inviwo/core/inviwo.h"
#include "inviwo/core/datastructures/representationconverter.h"

namespace inviwo {

    class RepresentationConverterFactory {

    public:
        RepresentationConverterFactory();
        virtual ~RepresentationConverterFactory();

        void registerRepresentationConverter(RepresentationConverter* representationConverter);
        RepresentationConverter* getRepresentationConverter(DataRepresentation* source, DataRepresentation* destination);

    private:
        std::vector<RepresentationConverter*> representationConverters_;
    };

} // namespace

#endif // IVW_REPRESENTATIONCONVERTERFACTORY_H
