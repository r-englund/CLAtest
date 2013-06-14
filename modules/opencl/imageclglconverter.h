#ifndef IVW_IMAGECLGLCONVERTER_H 
#define IVW_IMAGECLGLCONVERTER_H

#include <inviwo/core/datastructures/representationconverter.h>
#include <inviwo/core/datastructures/image/imageram.h>
#include <inviwo/core/datastructures/image/imageramconverter.h> 
#include <inviwo/core/datastructures/image/imagedisk.h>
#include <modules/opengl/image/imageglconverter.h> 
#include <modules/opencl/imagecl.h>
#include <modules/opencl/imageclgl.h>

namespace inviwo {

    class IVW_MODULE_OPENCL_API ImageRAM2CLGLConverter : public RepresentationConverterPackage<ImageCLGL> {

    public:
        ImageRAM2CLGLConverter();
        virtual ~ImageRAM2CLGLConverter() {};
    };

    class IVW_MODULE_OPENCL_API ImageDisk2CLGLConverter : public RepresentationConverterPackage<ImageCLGL> {
    
    public:
        ImageDisk2CLGLConverter() : RepresentationConverterPackage<ImageCLGL>() {
            addConverter(new ImageDisk2RAMConverter());
            addConverter(new ImageRAM2CLGLConverter());
        };
        virtual ~ImageDisk2CLGLConverter() {};
    };

    class IVW_MODULE_OPENCL_API ImageCLGL2RAMConverter : public RepresentationConverterType<ImageRAM> {

    public:
        ImageCLGL2RAMConverter();
        virtual ~ImageCLGL2RAMConverter(){};
        inline bool canConvert(const DataRepresentation* source) {
            if (!dynamic_cast<const ImageCLGL*>(source)) {
                return false;
            }
            return true;
        }
        DataRepresentation* createFrom(const DataRepresentation* source);
    };

    class IVW_MODULE_OPENCL_API ImageGL2CLGLConverter : public RepresentationConverterType<ImageCLGL> {

    public:
        ImageGL2CLGLConverter() : RepresentationConverterType<ImageCLGL>() {};
        virtual ~ImageGL2CLGLConverter() {};

        inline bool canConvert(const DataRepresentation* source) {
            if (!dynamic_cast<const ImageGL*>(source)) {
                return false;
            }
            return true;
        }

        DataRepresentation* createFrom(const DataRepresentation* source);
    };

    class IVW_MODULE_OPENCL_API ImageCLGL2CLConverter : public RepresentationConverterType<ImageCL> {

    public:
        ImageCLGL2CLConverter() : RepresentationConverterType<ImageCL>() {};
        virtual ~ImageCLGL2CLConverter() {};
        inline bool canConvert(const DataRepresentation* source) {
            if (!dynamic_cast<const ImageCLGL*>(source)) {
                return false;
            }
            return true;
        }

        DataRepresentation* createFrom(const DataRepresentation* source);
    };



} // namespace

#endif // IVW_IMAGECLGLCONVERTER_H
