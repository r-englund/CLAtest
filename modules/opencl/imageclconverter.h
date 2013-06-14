#ifndef IVW_IMAGECLCONVERTER_H
#define IVW_IMAGECLCONVERTER_H

#include <inviwo/core/datastructures/representationconverter.h>
#include <inviwo/core/datastructures/image/imageram.h>
#include <inviwo/core/datastructures/image/imageramconverter.h> 
#include <inviwo/core/datastructures/image/imagedisk.h>
#include <modules/opengl/image/imageglconverter.h> 
#include <modules/opencl/imagecl.h>

namespace inviwo {

    class IVW_MODULE_OPENCL_API ImageRAM2CLConverter : public RepresentationConverterType<ImageCL> {

    public:
        ImageRAM2CLConverter();
        virtual ~ImageRAM2CLConverter() {};

        inline bool canConvert(const DataRepresentation* source) {
            if (!dynamic_cast<const ImageRAM*>(source)) {
                return false;
            }
            return true;
        }

        DataRepresentation* createFrom(const DataRepresentation* source);
    };

    class IVW_MODULE_OPENCL_API ImageDisk2CLConverter : public RepresentationConverterPackage<ImageCL> {
    
    public:
        ImageDisk2CLConverter() : RepresentationConverterPackage<ImageCL>() {
            addConverter(new ImageDisk2RAMConverter());
            addConverter(new ImageRAM2CLConverter());
        };
        virtual ~ImageDisk2CLConverter() {};
    };

    class IVW_MODULE_OPENCL_API ImageCL2RAMConverter : public RepresentationConverterType<ImageRAM> {

    public:
        ImageCL2RAMConverter();
        virtual ~ImageCL2RAMConverter(){};

        inline bool canConvert(const DataRepresentation* source) {
            if (!dynamic_cast<const ImageCL*>(source)) {
                return false;
            }
            return true;
        }

        DataRepresentation* createFrom(const DataRepresentation* source);
    };

    class IVW_MODULE_OPENCL_API ImageGL2CLConverter : public RepresentationConverterPackage<ImageCL> {

    public:
        ImageGL2CLConverter() : RepresentationConverterPackage<ImageCL>() {
            addConverter(new ImageGL2RAMConverter());
            addConverter(new ImageRAM2CLConverter());
        };
        virtual ~ImageGL2CLConverter() {};
    };

    class IVW_MODULE_OPENCL_API ImageCL2GLConverter : public RepresentationConverterPackage<ImageGL> {

    public:
        ImageCL2GLConverter() : RepresentationConverterPackage<ImageGL>() {
            addConverter(new ImageCL2RAMConverter());
            addConverter(new ImageRAM2GLConverter());
        };
        virtual ~ImageCL2GLConverter() {};
    };



} // namespace

#endif // IVW_IMAGECLCONVERTER_H
