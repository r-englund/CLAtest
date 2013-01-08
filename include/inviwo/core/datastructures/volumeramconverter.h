#ifndef IVW_VOLUMERAMCONVERTER_H
#define IVW_VOLUMERAMCONVERTER_H

#include "inviwo/core/datastructures/representationconverter.h"
#include "inviwo/core/datastructures/volumeram.h"
#include "inviwo/core/datastructures/volumedisk.h"

namespace inviwo {

    class VolumeDisk2RAMConverter : public RepresentationConverterType<VolumeRAM> {

    public:
        VolumeDisk2RAMConverter();
        virtual ~VolumeDisk2RAMConverter();

        inline bool canConvert(DataRepresentation* source) {
            if (dynamic_cast<VolumeDisk*>(source)) return true;
            return false;
        }

        DataRepresentation* convert(DataRepresentation* source);
    };

} // namespace

#endif // IVW_VOLUMERAMCONVERTER_H
