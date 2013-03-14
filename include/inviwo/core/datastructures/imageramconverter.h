#ifndef IVW_IMAGERAMCONVERTER_H
#define IVW_IMAGERAMCONVERTER_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/datastructures/representationconverter.h>
#include <inviwo/core/datastructures/imageram.h>
#include <inviwo/core/datastructures/imagedisk.h>

namespace inviwo {

    class IVW_CORE_API ImageDisk2RAMConverter : public RepresentationConverterType<ImageRAM> {

    public:
        ImageDisk2RAMConverter();
        virtual ~ImageDisk2RAMConverter();

        inline bool canConvert(DataRepresentation* source) {
            if (dynamic_cast<ImageRAM*>(source)) return true;
            return false;
        }

        DataRepresentation* convert(DataRepresentation* source);
    };

} // namespace

#endif // IVW_IMAGERAMCONVERTER_H
