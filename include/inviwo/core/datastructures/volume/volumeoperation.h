#ifndef IVW_VOLUMEOPERATION_H
#define IVW_VOLUMEOPERATION_H

#include <inviwo/core/datastructures/dataoperation.h>

namespace inviwo {

class VolumeRepresentation;

class IVW_CORE_API VolumeOperation : public DataOperation {
public:
    VolumeOperation(const VolumeRepresentation* in) : DataOperation(), in_(in){}
    virtual ~VolumeOperation(){}

    const VolumeRepresentation* getInputVolume() { return in_; }

private:
    const VolumeRepresentation* in_;
};

} // namespace

#endif // IVW_VOLUMEOPERATION_H
