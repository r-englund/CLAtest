#ifndef IVW_IMAGEGLCONVERTER_H
#define IVW_IMAGEGLCONVERTER_H

#include "modules/opengl/openglmoduledefine.h"
#include "inviwo/core/datastructures/representationconverter.h"
#include "inviwo/core/datastructures/imageram.h"
#include "imagegl.h"

namespace inviwo {

    class IVW_MODULE_OPENGL_API ImageRAM2GLConverter : public RepresentationConverterType<ImageGL> {

    public:
        ImageRAM2GLConverter();
        virtual ~ImageRAM2GLConverter();

        inline bool canConvert(DataRepresentation* source) {
            if (dynamic_cast<ImageRAM*>(source)) return true;
            return false;
        }

         DataRepresentation* convert(DataRepresentation* source);
    };

} // namespace

#endif // IVW_IMAGEGLCONVERTER_H
