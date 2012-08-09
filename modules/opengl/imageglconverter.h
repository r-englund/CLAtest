#ifndef IVW_IMAGEGLCONVERTER_H
#define IVW_IMAGEGLCONVERTER_H

#include "inviwo/core/datastructures/representationconverter.h"
#include "inviwo/core/datastructures/imageram.h"
#include "imagegl.h"

namespace inviwo {

    class ImageGLConverter : public RepresentationConverter {

    public:
        ImageGLConverter();
        virtual ~ImageGLConverter();

        inline bool canConvert(DataRepresentation* source, DataRepresentation* destination) {
            return (dynamic_cast<ImageRAM*>(source) && dynamic_cast<ImageGL*>(destination));
        }

        void convert(DataRepresentation* source, DataRepresentation* destination);
    };

} // namespace

#endif // IVW_IMAGEGLCONVERTER_H
