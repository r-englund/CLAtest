#ifndef IVW_BUFFERCL_CONVERTER_H
#define IVW_BUFFERCL_CONVERTER_H

#include <inviwo/core/common/inviwo.h>
#include <modules/opencl/buffer/buffercl.h>
#include <modules/opencl/inviwoopencl.h>
#include <modules/opencl/openclmoduledefine.h>

namespace inviwo {


class IVW_MODULE_OPENCL_API BufferRAM2CLConverter : public RepresentationConverterType<BufferCL> {

public:
    BufferRAM2CLConverter();
    virtual ~BufferRAM2CLConverter();

    inline bool canConvertFrom(const DataRepresentation* source) const {
        return dynamic_cast<const BufferRAM*>(source) != NULL;
    }
    DataRepresentation* createFrom(const DataRepresentation* source);
    void update(const DataRepresentation* source, DataRepresentation* destination);
};

class IVW_MODULE_OPENCL_API BufferCL2RAMConverter : public RepresentationConverterType<BufferRAM> {

public:
    BufferCL2RAMConverter();
    virtual ~BufferCL2RAMConverter();

    inline bool canConvertFrom(const DataRepresentation* source) const {
        return dynamic_cast<const BufferCL*>(source) != NULL;
    }
    DataRepresentation* createFrom(const DataRepresentation* source);
    void update(const DataRepresentation* source, DataRepresentation* destination);
};

} // namespace inviwo

#endif // IVW_BUFFERCL_CONVERTER_H