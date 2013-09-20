#include <inviwo/core/datastructures/image/image.h>
#include <inviwo/core/datastructures/image/imagerepresentation.h>
#include <inviwo/core/datastructures/image/imagedisk.h>
#include <inviwo/core/datastructures/image/imageram.h>

namespace inviwo {

	
Image::Image(uvec2 dimensions, ImageType comb, const DataFormatBase* format, bool allowMissingLayers) : 
	StructuredData<2>(dimensions, format),
	imageType_(comb), 
	allowMissingLayers_(allowMissingLayers) {}

Data* Image::clone() const {
    Image* newImage = new Image(getDimension(), getImageType(), getDataFormat());
    
    //Do not copy all representations.
    //copyRepresentations(newImage);

    newImage->resize(getDimension());
    return newImage;
}

Image::~Image() {
    for (size_t i=0; i<representations_.size(); i++) {
        delete representations_[i];
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
    setAllRepresentationsAsInvalid();
}


uvec2 Image::getDimension() const{
	return StructuredData<2>::getDimension();
}
void  Image::setDimension(const uvec2& dim){
	StructuredData<2>::setDimension(dim);
}



void Image::resizeImageRepresentations(Image* targetImage, uvec2 targetDim) {
    //TODO: check if getClassName() is necessary.
    //TODO: And also need to be tested on multiple representations_ such as ImageRAM, ImageDisk etc.,
    //TODO: optimize the code
    targetImage->resize(targetDim);
    ImageRepresentation* imageRepresentation = 0;
    ImageRepresentation* targetRepresentation = 0;
    std::vector<DataRepresentation*> &targetRepresentations = targetImage->representations_;

    if (targetRepresentations.size()) {
        for (int i=0; i<static_cast<int>(representations_.size()); i++) {
            imageRepresentation = dynamic_cast<ImageRepresentation*>(representations_[i]) ;        
            ivwAssert(imageRepresentation!=0, "Only image representations should be used here.");
            if (isRepresentationValid(i)) {
                int numberOfTargets = static_cast<int>(targetRepresentations.size());
                for (int j=0; j<numberOfTargets; j++) {
                    targetRepresentation = dynamic_cast<ImageRepresentation*>(targetRepresentations[j]) ;
                    ivwAssert(targetRepresentation!=0, "Only image representations should be used here.");
                    if (imageRepresentation->getClassName()==targetRepresentation->getClassName()) {
                        if (imageRepresentation->copyAndResizeImage(targetRepresentation)) {
                            targetImage->setRepresentationAsValid(j);
                            targetImage->lastValidRepresentation_ = targetRepresentations[j];
                        }
                    }
                }
            }
        }
    }
    else {
        ivwAssert(lastValidRepresentation_!=0, "Last valid representation is expected.");
        targetImage->setAllRepresentationsAsInvalid();
        targetImage->createDefaultRepresentation();
        ImageRepresentation* lastValidRepresentation = dynamic_cast<ImageRepresentation*>(lastValidRepresentation_);
        ImageRepresentation* cloneOfLastValidRepresentation = dynamic_cast<ImageRepresentation*>(lastValidRepresentation->clone());        
        targetImage->addRepresentation(cloneOfLastValidRepresentation);        

       targetImage->resize(targetDim);
       if (lastValidRepresentation->copyAndResizeImage(cloneOfLastValidRepresentation)) {
            targetImage->setRepresentationAsValid(static_cast<int>(targetImage->representations_.size())-1);
            targetImage->lastValidRepresentation_ = cloneOfLastValidRepresentation;
       }
    }
}

void Image::setInputSource(ImageLayerType layer, const Image* src){
    inputSources_[layer] = src;
}

void Image::setAllowMissingLayers(bool allowMissingLayers){
    allowMissingLayers_ = allowMissingLayers;
}

void Image::createDefaultRepresentation() const{
	representations_.push_back(createImageRAM((uvec2)getDimension(), getImageType(), getDataFormat()));
}

void Image::editableRepresentationCreated() const{
    if(!allowMissingLayers_){
        ImageRepresentation* lastValidRepresentation = dynamic_cast<ImageRepresentation*>(lastValidRepresentation_);
        ImageSourceMap::const_iterator it;
        if(!typeContainsColor(getImageType())){
            it = inputSources_.find(COLOR_LAYER);
            if(it != inputSources_.end())
                lastValidRepresentation->useInputSource(COLOR_LAYER, it->second);
            else
                lastValidRepresentation->createAndAddLayer(COLOR_LAYER);
        }
        if(!typeContainsDepth(getImageType())){
            it = inputSources_.find(DEPTH_LAYER);
            if(it != inputSources_.end())
                lastValidRepresentation->useInputSource(DEPTH_LAYER, it->second);
            else
                lastValidRepresentation->createAndAddLayer(DEPTH_LAYER);
        }
        if(!typeContainsPicking(getImageType())){
            it = inputSources_.find(PICKING_LAYER);
            if(it != inputSources_.end())
                lastValidRepresentation->useInputSource(PICKING_LAYER, it->second);
            else
                lastValidRepresentation->createAndAddLayer(PICKING_LAYER);
        }
    }
}

} // namespace
