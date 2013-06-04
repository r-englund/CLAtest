#ifndef IVW_VOLUME_H
#define IVW_VOLUME_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/data.h>
#include <inviwo/core/datastructures/volume/volumerepresentation.h>

namespace inviwo {

    class IVW_CORE_API Volume : public Data3D {

    public:
        Volume(uvec3 dimensions = uvec3(128,128,128), DataFormatBase format = DataUINT8());
        Volume(VolumeRepresentation*);
        Volume(VolumeRepresentation*, const Volume*);
        virtual ~Volume();
        Data* clone() const;        
        void setOffset(ivec3); 
        ivec3 getOffset() const;
    protected:
        void createDefaultRepresentation() const;
    };

} // namespace

#endif // IVW_VOLUME_H
