#ifndef IVW_BUFFERCLGLCONVERTER_H 
#define IVW_BUFFERCLGLCONVERTER_H

#include <inviwo/core/common/inviwo.h>
#include <modules/opencl/buffer/buffercl.h>
#include <modules/opencl/buffer/bufferclgl.h>
#include <modules/opencl/inviwoopencl.h>
#include <modules/opencl/openclmoduledefine.h>
#include <modules/opengl/buffer/bufferglconverter.h>

namespace inviwo {

class IVW_MODULE_OPENCL_API BufferRAM2CLGLConverter : public RepresentationConverterPackage<BufferCLGL> {

public:
    BufferRAM2CLGLConverter();
    virtual ~BufferRAM2CLGLConverter() {};
};



class IVW_MODULE_OPENCL_API BufferCLGL2RAMConverter : public RepresentationConverterType<BufferRAM> {

public:
    BufferCLGL2RAMConverter();
    virtual ~BufferCLGL2RAMConverter(){};

    inline bool canConvertFrom(const DataRepresentation* source) const {
        return dynamic_cast<const BufferCLGL*>(source) != NULL;
    }
    DataRepresentation* createFrom(const DataRepresentation* source);
    void update(const DataRepresentation* source, DataRepresentation* destination);
};

class IVW_MODULE_OPENCL_API BufferGL2CLGLConverter : public RepresentationConverterType<BufferCLGL> {

public:
    BufferGL2CLGLConverter() : RepresentationConverterType<BufferCLGL>() {};
    virtual ~BufferGL2CLGLConverter() {};

    inline bool canConvertFrom(const DataRepresentation* source) const {
        return dynamic_cast<const BufferGL*>(source) != NULL;
    }
    DataRepresentation* createFrom(const DataRepresentation* source);
    void update(const DataRepresentation* source, DataRepresentation* destination);
};

class IVW_MODULE_OPENCL_API BufferCLGL2CLConverter : public RepresentationConverterType<BufferCL> {
public:
    BufferCLGL2CLConverter() : RepresentationConverterType<BufferCL>() {};
    virtual ~BufferCLGL2CLConverter() {};

    inline bool canConvertFrom(const DataRepresentation* source) const {
        return dynamic_cast<const BufferCLGL*>(source) != NULL;
    }
    DataRepresentation* createFrom(const DataRepresentation* source);
    void update(const DataRepresentation* source, DataRepresentation* destination);
};

class IVW_MODULE_OPENCL_API BufferCL2CLGLConverter : public RepresentationConverterPackage<BufferCLGL> {
public:
    BufferCL2CLGLConverter();
    virtual ~BufferCL2CLGLConverter() {};
};

//class IVW_MODULE_OPENCL_API BufferDisk2CLGLConverter : public RepresentationConverterPackage<BufferCLGL> {
//
//public:
//    BufferDisk2CLGLConverter() : RepresentationConverterPackage<BufferCLGL>() {
//        addConverter(new BufferDisk2RAMConverter());
//        addConverter(new BufferRAM2GLConverter());
//        addConverter(new BufferGL2CLGLConverter());
//    };
//    virtual ~BufferDisk2CLGLConverter() {};
//};

} // namespace

#endif // IVW_BUFFERCLGLCONVERTER_H
