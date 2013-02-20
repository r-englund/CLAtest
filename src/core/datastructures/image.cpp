#include <inviwo/core/datastructures/image.h>
#include <inviwo/core/datastructures/imagerepresentation.h>

#include <inviwo/core/datastructures/imageram.h>

namespace inviwo {

    Image::Image() : Data() {
        dimensions_ = uvec2(256,256);
        representations_.clear();
        addRepresentation(new ImageRAM(dimensions_));
    }

    Image::Image(uvec2 dimensions)
        : Data(),
          dimensions_(dimensions) {
        representations_.clear();
        addRepresentation(new ImageRAM(dimensions_));
    }

    Data* Image::clone() {
        Image* newImage = new Image(dimensions_);
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
        dimensions_ = dimensions;
        for (size_t i=0; i<representations_.size(); i++) {
            ImageRepresentation* imageRepresentation = dynamic_cast<ImageRepresentation*>(representations_[i]) ;
            if ( imageRepresentation ) {
                imageRepresentation->resize(dimensions_);
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

} // namespace
