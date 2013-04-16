#ifndef IVW_VOLUMERAM_H
#define IVW_VOLUMERAM_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/inviwo.h>
#include <inviwo/core/datastructures/volumerepresentation.h>

namespace inviwo {

class IVW_CORE_API VolumeRAM : public VolumeRepresentation {

public:
    VolumeRAM(uvec3 dimension = uvec3(128,128,128), VolumeRepresentation::VolumeBorders border = VolumeRepresentation::VolumeBorders(), DataFormatBase format = DataFormatBase());
    virtual ~VolumeRAM();
    virtual void performOperation(DataOperation*) const = 0;
    virtual void initialize();
    virtual void deinitialize();
    virtual DataRepresentation* clone() const = 0;
    virtual void* getData();
    virtual const void* getData() const;
protected:
    void* data_;
};

} // namespace

#endif // IVW_VOLUMERAM_H
