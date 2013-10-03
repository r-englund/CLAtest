#ifndef IVW_IMAGERAMCONVERTER_H
#define IVW_IMAGERAMCONVERTER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/representationconverter.h>
#include <inviwo/core/datastructures/image/imageram.h>
#include <inviwo/core/datastructures/image/imagedisk.h>

namespace inviwo {

    class IVW_CORE_API ImageDisk2RAMConverter : public RepresentationConverterType<ImageRAM> {

    public:
        ImageDisk2RAMConverter();
        virtual ~ImageDisk2RAMConverter();

        inline bool canConvertFrom(const DataRepresentation* source) const {
            return dynamic_cast<const ImageDisk*>(source) != NULL;
        }
        DataRepresentation* createFrom(const DataRepresentation* source);
        void update(const DataRepresentation* source, DataRepresentation* destination);
    };

} // namespace

#endif // IVW_IMAGERAMCONVERTER_H
