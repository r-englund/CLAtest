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
    currentImageIndex_("currentImageIndex", "Image index", 1, 1, 1, 1)
{
    addPort(outport_);    
    addProperty(imageFileDirectory_);
    addProperty(findFilesButton_);
    addProperty(currentImageIndex_);

    findFilesButton_.registerClassMemberFunction(this, &ImageSourceSeries::onFindFiles);
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
    if(files.size())
        currentImageIndex_.setMaxValue(static_cast<const int>(files.size()));

    //imageFilesInDirectory_.clearOptions();
    std::vector<std::string> ids;
    std::vector<std::string> displayNames;
    for (size_t i=0; i<files.size(); i++) {
        std::string displayName = URLParser::getFileNameWithExtension(files[i]); 
        ids.push_back(displayName+"_id");
        displayNames.push_back(displayName);
    }

    if(!files.size()) {
        ids.push_back("noImage");
        displayNames.push_back("noImage");
        files.push_back("");
    }
 
    currentImageIndex_.set(static_cast<const int>(files.size()));
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
