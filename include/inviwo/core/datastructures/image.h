#ifndef IVW_IMAGE_H
#define IVW_IMAGE_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/data.h>

namespace inviwo {

    class IVW_CORE_API Image : public Data {

    public:
        Image();
        Image(uvec2 dimensions);
        virtual ~Image();
        void resize(uvec2 dimensions);
        uvec2 size() const {return dimensions_;}
        virtual Data* clone() const;
        void resizeImageRepresentations(Image* targetImage, uvec2 targetDim);
    protected:
        void createDefaultRepresentation() const;
    private:
        uvec2 dimensions_;
    };




} // namespace

#endif // IVW_IMAGE_H
