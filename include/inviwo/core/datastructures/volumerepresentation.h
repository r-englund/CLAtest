#ifndef IVW_VOLUMEREPRESENTATION_H
#define IVW_VOLUMEREPRESENTATION_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/datarepresentation.h>

namespace inviwo {

    class IVW_CORE_API VolumeRepresentation : public DataRepresentation {

    public:
        /// struct volume borders
        struct VolumeBorders{
            uvec3 llf;
            uvec3 urb;
            size_t numVoxels;
            bool hasBorder;

            VolumeBorders() : llf(uvec3(0,0,0)), urb(uvec3(0,0,0)), numVoxels(0), hasBorder(false){}
            VolumeBorders(size_t front, size_t back, size_t left, size_t right, size_t lower, size_t upper) : llf(uvec3(front, left, lower)), urb(uvec3(back, right, upper)){}
            VolumeBorders(const uvec3& llfBorder, const uvec3& urbBorder) : llf(llfBorder), urb(urbBorder){}
            bool operator== (const VolumeBorders &vb) const{ return (llf == vb.llf && urb == vb.urb);}
            bool operator!= (const VolumeBorders &vb) const{ return (llf != vb.llf || urb != vb.urb);}
        };

        VolumeRepresentation(uvec3 dimension);
        VolumeRepresentation(uvec3 dimension, DataFormatBase format);
        VolumeRepresentation(uvec3 dimension, DataFormatBase format, VolumeBorders border);
        virtual ~VolumeRepresentation();
        virtual void performOperation(DataOperation*) const {};
        virtual VolumeBorders getBorder() const {return borders_;}
        virtual uvec3 getBorderLLF() const {return borders_.llf;}
        virtual uvec3 getBorderURB() const {return borders_.urb;}
        virtual uvec3 getDimensions() const {return dimensions_;}
        virtual uvec3 getDimensionsWithBorder() const { return dimensions_+getBorderLLF()+getBorderURB(); }
        virtual bool hasBorder() const {return borders_.hasBorder;}
    protected:
        uvec3 dimensions_;
        VolumeBorders borders_;
    };

} // namespace

#endif // IVW_VOLUMEREPRESENTATION_H
