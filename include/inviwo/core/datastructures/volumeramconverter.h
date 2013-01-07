#ifndef IVW_VOLUMERAMCONVERTER_H
#define IVW_VOLUMERAMCONVERTER_H

#include "inviwo/core/datastructures/representationconverter.h"
#include "inviwo/core/datastructures/volumeram.h"
#include "inviwo/core/datastructures/volumedisk.h"

namespace inviwo {

    class VolumeRAMConverter : public RepresentationConverterType<VolumeRAM> {

    public:
        VolumeRAMConverter();
        virtual ~VolumeRAMConverter();

        inline bool canConvert(DataRepresentation* source) {
            if (dynamic_cast<VolumeDisk*>(source)) return true;
            return false;
        }

        DataRepresentation* convert(DataRepresentation* source);
    };

} // namespace

#endif // IVW_VOLUMERAMCONVERTER_H
