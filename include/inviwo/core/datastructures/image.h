#ifndef IVW_IMAGE_H
#define IVW_IMAGE_H

#include "inviwo/core/datastructures/data.h"

namespace inviwo {

    class Image : public Data {

    public:
        Image();
        Image(ivec2 dimensions);
        virtual ~Image();

        void resize(ivec2 dimensions);
        ivec2 size() {return dimensions_;}
        virtual Data* clone();
        void resizeImageRepresentations(Image* targetImage, ivec2 targetDim);

    private:
        ivec2 dimensions_;
    };

} // namespace

#endif // IVW_IMAGE_H
