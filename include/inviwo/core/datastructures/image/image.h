#ifndef IVW_IMAGE_H
#define IVW_IMAGE_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/data.h>
#include <inviwo/core/datastructures/image/imagetypes.h>

namespace inviwo {

class IVW_CORE_API Image : public Data2D {

public:
    Image(uvec2 dimensions = uvec2(256,256), DataFormatBase format = DataVec4UINT8(), ImageType type = COLOR_DEPTH);
    virtual ~Image();
    void resize(uvec2 dimensions);
    virtual Data* clone() const;
    void resizeImageRepresentations(Image* targetImage, uvec2 targetDim);
    ImageType getImageType() const { return imageType_; }
protected:
    void createDefaultRepresentation() const;
private:
    const Image* src_;
    ImageType imageType_;
};

} // namespace

#endif // IVW_IMAGE_H
