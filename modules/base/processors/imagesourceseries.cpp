#include "imagesourceseries.h"

namespace inviwo {

ProcessorClassName(ImageSourceSeries, "ImageSourceSeries"); 
ProcessorCategory(ImageSourceSeries, "Data Input");
ProcessorCodeState(ImageSourceSeries, CODE_STATE_EXPERIMENTAL);

ImageSourceSeries::ImageSourceSeries()
    : Processor(),
    outport_("image.outport"),
    imageFileDirectory_("imageFileDirectory", "Image file directory", IVW_DIR+"data/images"),
    findFilesButton_("findFiles", "Find Files"),
    currentImageIndex_("currentImageIndex", "Image index", 1, 1, 1, 1),
    imageFilesInDirectory_("imageFiles", "Current File")
{
    addPort(outport_);    
    addProperty(imageFileDirectory_);
    addProperty(findFilesButton_);
    addProperty(currentImageIndex_);

    findFilesButton_.registerClassMemberFunction(this, &ImageSourceSeries::onFindFiles);

    //TODO: following settings leads to crash. under investigation.
    //addProperty(imageFilesInDirectory_);
    //imageFilesInDirectory_.setReadOnly(true);    
    //imageFileDirectory_.onChange(this, &ImageSourceSeries::onFindFiles);
}

ImageSourceSeries::~ImageSourceSeries() {}

void ImageSourceSeries::initialize() {
    Processor::initialize();
}

void ImageSourceSeries::deinitialize() {
    Processor::deinitialize();
}

void ImageSourceSeries::onFindFiles() {
    std::vector<std::string> files = imageFileDirectory_.getFiles();    

    //TODO: following settings leads to crash. under investigation.
    /*imageFilesInDirectory_.clearOptions();
    for (size_t i=0; i<files.size(); i++) {
        std::string identifier = URLParser::getFileNameWithExtension(files[i]); 
        imageFilesInDirectory_.addOption(identifier+"_id", identifier);
    }    
    imageFilesInDirectory_.setSelectedOption(files.size()-1);*/

    currentImageIndex_.setMaxValue(files.size());
    currentImageIndex_.set(files.size());

}

/**
* Creates a ImageDisk representation if there isn't an object already defined.
**/
void ImageSourceSeries::process() {
	Image* outImage = outport_.getData(); 
    if (outImage){
        
        std::vector<std::string> fileNames = imageFileDirectory_.getFiles();
        size_t currentIndex = currentImageIndex_.get();

        if (!fileNames.size()) {
            LogWarn("No image found in the directory.");
            return;
        }

        if (currentIndex > fileNames.size()) {
            LogWarn("Current index exceeded the number of files.");
            return;
        }

        if (currentIndex==0) {
            LogWarn("Invalid index");
            return;
        }

        std::string currentFileName = fileNames[currentIndex-1];
        ImageDisk* outImageDisk = outImage->getEditableRepresentation<ImageDisk>();
        if (!outImageDisk || outImageDisk->getSourceFile() != currentFileName){ 
            outImageDisk = new ImageDisk(currentFileName);
            outImage->clearRepresentations();
            outImage->addRepresentation(outImageDisk);
        }

        //Original image dimension loaded from disk may differ from requested dimension.
        outImageDisk->resize(outImage->getDimension());
    }
}

} // namespace
