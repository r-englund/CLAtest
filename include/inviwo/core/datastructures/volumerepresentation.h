#ifndef IVW_VOLUMEREPRESENTATION_H
#define IVW_VOLUMEREPRESENTATION_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/datastructures/datarepresentation.h>

namespace inviwo {

    class IVW_CORE_API VolumeRepresentation : public DataRepresentation {

    public:
        VolumeRepresentation(ivec3 dimension, std::string format);
        virtual ~VolumeRepresentation();
        virtual ivec3 dimension() {return dimensions_;}
        virtual std::string getDataFormat() {return dataFormat_;}
    protected:
        ivec3 dimensions_;
        std::string dataFormat_;
    };

} // namespace

#endif // IVW_VOLUMEREPRESENTATION_H
