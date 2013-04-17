#ifndef IVW_IMAGERAMCONVERTER_H
#define IVW_IMAGERAMCONVERTER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/representationconverter.h>
#include <inviwo/core/datastructures/imageram.h>
#include <inviwo/core/datastructures/imagedisk.h>

namespace inviwo {

    class IVW_CORE_API ImageDisk2RAMConverter : public RepresentationConverterType<ImageRAM> {

    public:
        ImageDisk2RAMConverter();
        virtual ~ImageDisk2RAMConverter();
        
        /**
        * Checks if it is possible to convert the data representation by testing if a
        * cast is possible.
        * @param source is the DataRepresentation to test for convertion possibility.
        * @return boolean value stating if a convertion is possible.
        **/
        inline bool canConvert(DataRepresentation* source) {
            if (dynamic_cast<ImageDisk*>(source)) return true;
            return false;
        }

        DataRepresentation* convert(DataRepresentation* source);
    };

} // namespace

#endif // IVW_IMAGERAMCONVERTER_H
