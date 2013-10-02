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
    
        uvec3 getDimension() const;
        void setDimension(const uvec3& dim);

        void setOffset(const vec3& offset); 
		vec3 getOffset() const;
	
        mat3 getBasis() const;
        void setBasis(const mat3& basis);

        mat4 getBasisAndOffset() const;
        void setBasisAndOffset(const mat4& mat);

        mat4 getWorldTransform() const;
        void setWorldTransform(const mat4& mat);

    protected:
        void createDefaultRepresentation() const;
    };

} // namespace

#endif // IVW_VOLUME_H
