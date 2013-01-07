#ifndef IVW_REPRESENTATIONCONVERTER_H
#define IVW_REPRESENTATIONCONVERTER_H

#include "inviwo/core/datastructures/datarepresentation.h"

namespace inviwo {

    class RepresentationConverter {

    public:
        RepresentationConverter();
        virtual ~RepresentationConverter();

        virtual bool canConvert(DataRepresentation* source) = 0;
        virtual DataRepresentation* convert(DataRepresentation* source) = 0;        
    };


    template <typename T>
    class RepresentationConverterType : public RepresentationConverter{
    public:
        template <typename U>
        bool isValidConverter() {
            if (dynamic_cast<RepresentationConverterType<U>*>(this))
                return true;
            return false;
        }
    };

} // namespace

#endif // IVW_REPRESENTATIONCONVERTER_H
