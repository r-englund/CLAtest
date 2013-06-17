#ifndef IVW_VOLUMERAMCONVERTER_H
#define IVW_VOLUMERAMCONVERTER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/representationconverter.h>
#include <inviwo/core/datastructures/volume/volumeram.h>
#include <inviwo/core/datastructures/volume/volumedisk.h>

namespace inviwo {

    class IVW_CORE_API VolumeDisk2RAMConverter : public RepresentationConverterType<VolumeRAM> {

    public:
        VolumeDisk2RAMConverter();
        virtual ~VolumeDisk2RAMConverter();

        inline bool canConvertFrom(const DataRepresentation* source) const {
            return dynamic_cast<const VolumeDisk*>(source) != NULL;
        }

        DataRepresentation* createFrom(const DataRepresentation* source);
        void update(const DataRepresentation* source, DataRepresentation* destination);
    };

} // namespace

#endif // IVW_VOLUMERAMCONVERTER_H
