#ifndef IVW_IMAGE_H
#define IVW_IMAGE_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/data.h>
#include <inviwo/core/datastructures/image/imagetypes.h>

namespace inviwo {

class IVW_CORE_API Image : public Data2D {

public:
    Image(uvec2 dimensions = uvec2(256,256), ImageType type = COLOR_DEPTH, DataFormatBase format = DataVec4UINT8(), bool allowMissingLayers = true);
    virtual ~Image();
    void resize(uvec2 dimensions);
    virtual Data* clone() const;
    void resizeImageRepresentations(Image* targetImage, uvec2 targetDim);
    ImageType getImageType() const { return imageType_; }
    void setInputSource(ImageLayerType, const Image*);
    void setAllowMissingLayers(bool);
protected:
    void createDefaultRepresentation() const;
    void editableRepresentationCreated() const;
private:
    bool allowMissingLayers_;
    ImageType imageType_;
    typedef std::map<ImageLayerType, const Image*> ImageSourceMap;
    ImageSourceMap inputSources_;
};

} // namespace

#endif // IVW_IMAGE_H
