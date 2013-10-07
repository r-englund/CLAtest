#ifndef IVW_IMAGE_H
#define IVW_IMAGE_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/data.h>
#include <inviwo/core/datastructures/image/imagetypes.h>

namespace inviwo {

class IVW_CORE_API Image : public StructuredData<2> {

public:
    Image(uvec2 dimensions = uvec2(256,256), ImageType type = COLOR_DEPTH, const DataFormatBase* format = DataVec4UINT8::get(), bool allowMissingLayers = false);
    virtual ~Image();
    void resize(uvec2 dimensions);
    virtual Data* clone() const;
    void resizeImageRepresentations(Image* targetImage, uvec2 targetDim);
    ImageType getImageType() const { return imageType_; }
    void setInputSource(ImageLayerType, const Image*);
    void setAllowMissingLayers(bool);

	uvec2 getDimension() const;
	void setDimension(const uvec2& dim);

protected:
    void createDefaultRepresentation() const;
    void newEditableRepresentationCreated() const;
private:
    bool allowMissingLayers_;
    ImageType imageType_;
    typedef std::map<ImageLayerType, const Image*> ImageSourceMap;
    ImageSourceMap inputSources_;
};

} // namespace

#endif // IVW_IMAGE_H
