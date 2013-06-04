#include <inviwo/core/datastructures/image/image.h>
#include <inviwo/core/datastructures/image/imagerepresentation.h>
#include <inviwo/core/datastructures/image/imagedisk.h>
#include <inviwo/core/datastructures/image/imageram.h>

namespace inviwo {

Image::Image(uvec2 dimensions, DataFormatBase format) : Data2D(dimensions, format) {}

Data* Image::clone() const {
    Image* newImage = new Image(getDimension());
    copyRepresentations(newImage);
    return newImage;
}

Image::~Image() {
    for (size_t i=0; i<representations_.size(); i++) {
        representations_[i]->deinitialize();
    }
    representations_.clear();
}

void Image::resize(uvec2 dimensions) {
    setDimension(dimensions);
    for (size_t i=0; i<representations_.size(); i++) {
        ImageRepresentation* imageRepresentation = dynamic_cast<ImageRepresentation*>(representations_[i]) ;
        if ( imageRepresentation ) {
            imageRepresentation->resize(dimensions);
        }
    }
}

void Image::resizeImageRepresentations(Image* targetImage, uvec2 targetDim) {
    //TODO: check if getClassName() is necessary.
    //TODO: And also need to be tested on multiple representations_ such as ImageRAM, ImageDisk etc.,
    //TODO: optimize the code
    targetImage->resize(targetDim);
    ImageRepresentation* imageRepresentation = 0;
    ImageRepresentation* targetRepresentation = 0;
    std::vector<DataRepresentation*> &targetRepresentations = targetImage->representations_;
    for (size_t i=0; i<representations_.size(); i++) {
        imageRepresentation = dynamic_cast<ImageRepresentation*>(representations_[i]) ;
        ivwAssert(imageRepresentation!=0, "Only image representations should be used here.");
        for (size_t j=0; j<targetRepresentations.size(); j++) {                
            targetRepresentation = dynamic_cast<ImageRepresentation*>(targetRepresentations[j]) ;
            ivwAssert(targetRepresentation!=0, "Only image representations should be used here.");
            if (imageRepresentation->getClassName()==targetRepresentation->getClassName()) {
                imageRepresentation->copyAndResizeImage(targetRepresentation);
            }
        }
    }
}

void Image::createDefaultRepresentation() const{
    //representations_.push_back(new ImageDisk());
    representations_.push_back(createImage(getDimension(), getDataFormat()));
}

} // namespace
