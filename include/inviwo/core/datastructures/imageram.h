#ifndef IVW_IMAGERAM_H
#define IVW_IMAGERAM_H

#include "inviwo/core/datastructures/imagerepresentation.h"

namespace inviwo {

    class ImageRAM : public ImageRepresentation {

    public:
        ImageRAM();
        virtual ~ImageRAM();
        virtual void initialize();
        void deinitialize();
        DataRepresentation* clone();
        virtual std::string getClassName() const { return "ImageRAM"; }
        void copyAndResizeImage(DataRepresentation*){}
    };

} // namespace

#endif // IVW_IMAGERAM_H
