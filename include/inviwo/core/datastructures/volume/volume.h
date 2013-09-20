#ifndef IVW_VOLUME_H
#define IVW_VOLUME_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/data.h>
#include <inviwo/core/datastructures/volume/volumerepresentation.h>

namespace inviwo {

    class IVW_CORE_API Volume : public StructuredData<3> {
    public:
        Volume(uvec3 dimensions = uvec3(128,128,128), const DataFormatBase* format = DataUINT8::get());
        Volume(VolumeRepresentation*);
        Volume(VolumeRepresentation*, const Volume*);
        virtual ~Volume();
        Data* clone() const;        
        void setOffset(ivec3); 
		ivec3 getOffset() const;
	
		uvec3 getDimension() const;
		void setDimension(const uvec3& dim);

    protected:
        void createDefaultRepresentation() const;
    };

} // namespace

#endif // IVW_VOLUME_H
