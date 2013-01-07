#ifndef IVW_IMAGEGLCONVERTER_H
#define IVW_IMAGEGLCONVERTER_H

#include "inviwo/core/datastructures/representationconverter.h"
#include "inviwo/core/datastructures/imageram.h"
#include "imagegl.h"

namespace inviwo {

    class ImageGLConverter : public RepresentationConverterType<ImageGL> {

    public:
        ImageGLConverter();
        virtual ~ImageGLConverter();

        inline bool canConvert(DataRepresentation* source) {
            if (dynamic_cast<ImageRAM*>(source)) return true;
            return false;
        }

         DataRepresentation* convert(DataRepresentation* source);
    };

} // namespace

#endif // IVW_IMAGEGLCONVERTER_H
