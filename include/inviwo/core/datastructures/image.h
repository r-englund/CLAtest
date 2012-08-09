#ifndef IVW_IMAGE_H
#define IVW_IMAGE_H

#include "inviwo/core/datastructures/data.h"

namespace inviwo {

    class Image : public Data {

    public:
        Image();
        Image(ivec2 dimensions);
        virtual ~Image();

        void resize(ivec2 dimensions) {
            dimensions_ = dimensions;
            clearRepresentations();
        }

    private:
        ivec2 dimensions_;
    };

} // namespace

#endif // IVW_IMAGE_H
