#ifndef IVW_IMAGE_H
#define IVW_IMAGE_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/datastructures/data.h>

namespace inviwo {

    class IVW_CORE_API Image : public Data {

    public:
        Image();
        Image(uvec2 dimensions);
        virtual ~Image();
        void resize(uvec2 dimensions);
        uvec2 size() {return dimensions_;}
        virtual Data* clone();
        void resizeImageRepresentations(Image* targetImage, uvec2 targetDim);
    protected:
        void createDefaultRepresentation();
    private:
        uvec2 dimensions_;
    };




} // namespace

#endif // IVW_IMAGE_H
