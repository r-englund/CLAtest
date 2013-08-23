#ifndef IVW_IMAGEREPRESENTATION_H
#define IVW_IMAGEREPRESENTATION_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/datarepresentation.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/image/imagetypes.h>
#include <inviwo/core/datastructures/image/image.h>

namespace inviwo {

class IVW_CORE_API ImageRepresentation : public DataRepresentation {

friend class Image;

public:
    ImageRepresentation(uvec2 dimensions, ImageType type, DataFormatBase format);
    virtual ~ImageRepresentation();
    virtual void performOperation(DataOperation*) const {};
    virtual void resize(uvec2 dimensions);
    const uvec2& getDimensions() const {return dimensions_;}
    virtual bool copyAndResizeImage(DataRepresentation*) = 0;
    virtual DataRepresentation* clone() const = 0;
    virtual std::string getClassName() const { return "ImageRepresentation"; }
    ImageType getImageType() const { return imageType_; }
protected:
    virtual void useInputSource(ImageLayerType, const Image*) {}
    virtual void createAndAddLayer(ImageLayerType) {}

    uvec2 dimensions_;
    ImageType imageType_;
};

} // namespace

#endif // IVW_IMAGEREPRESENTATION_H
