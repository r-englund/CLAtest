#ifndef IVW_VOLUMEPORT_H
#define IVW_VOLUMEPORT_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/ports/datainport.h>
#include <inviwo/core/ports/dataoutport.h>
#include <inviwo/core/datastructures/volume/volume.h>

namespace inviwo {

class IVW_CORE_API VolumeInport : public DataInport<Volume> {

public:
    VolumeInport(std::string identifier,
                 PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT);
    virtual ~VolumeInport();

    void initialize();
    void deinitialize();

    uvec3 getColorCode() const;
};

class IVW_CORE_API VolumeOutport : public DataOutport<Volume> {

public:
    VolumeOutport(std::string identifier,
                  PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT);
    virtual ~VolumeOutport();

    void initialize();
    void deinitialize();

    uvec3 getColorCode() const;
};

} // namespace

#endif // IVW_VOLUMEPORT_H
