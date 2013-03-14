#include <inviwo/core/datastructures/imagedisk.h>
#include <modules/opengl/imageloader.h>

namespace inviwo {

    ImageDisk::ImageDisk()
        : ImageRepresentation()
    {}

	ImageDisk::ImageDisk(std::string url)
        : ImageRepresentation(){
		sourceFile_ = url;
        initialize();
	}

    ImageDisk::~ImageDisk() {}

    void ImageDisk::initialize(){
        if(!sourceFile_.empty()){
            dimensions_ = ImageLoader::imageDimensions(sourceFile_);
        }
    }

    void* ImageDisk::loadFileData(){
        return ImageLoader::loadImageToData(sourceFile_);  
    }

    void ImageDisk::deinitialize() {}

    DataRepresentation* ImageDisk::clone() {
        return new ImageDisk();
    }

} // namespace
