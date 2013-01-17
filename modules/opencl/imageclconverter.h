#ifndef IVW_IMAGECLCONVERTER_H
#define IVW_IMAGECLCONVERTER_H

#include "inviwo/core/datastructures/representationconverter.h"
#include "inviwo/core/datastructures/imageram.h"
// TODO: implement conversion when imageram converter exist
//#include "inviwo/core/datastructures/imageramconverter.h" 
#include "inviwo/core/datastructures/imagedisk.h"
#include "modules/opencl/imagecl.h"

namespace inviwo {

    class IVW_MODULE_OPENCL_API ImageRAM2CLConverter : public RepresentationConverterType<ImageCL> {

    public:
        ImageRAM2CLConverter();
        virtual ~ImageRAM2CLConverter() {};

        inline bool canConvert(DataRepresentation* source) {
            if (!dynamic_cast<ImageRAM*>(source)) {
                return false;
            }
            return true;
        }

        DataRepresentation* convert(DataRepresentation* source);
    };

    class IVW_MODULE_OPENCL_API ImageDisk2CLConverter : public RepresentationConverterPackage<ImageCL> {
    
    public:
        ImageDisk2CLConverter() : RepresentationConverterPackage<ImageCL>(){
            //addConverter(new ImageDisk2RAMConverter());
            addConverter(new ImageRAM2CLConverter());
        };
        virtual ~ImageDisk2CLConverter() {};
    };

    class IVW_MODULE_OPENCL_API ImageCL2RAMConverter : public RepresentationConverterType<ImageRAM> {

    public:
        ImageCL2RAMConverter();
        virtual ~ImageCL2RAMConverter(){};

        inline bool canConvert(DataRepresentation* source) {
            if (!dynamic_cast<ImageCL*>(source)) {
                return false;
            }
            return true;
        }

        DataRepresentation* convert(DataRepresentation* source);
    };



} // namespace

#endif // IVW_IMAGECLCONVERTER_H
