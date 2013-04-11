#include <inviwo/core/datastructures/imagedisk.h>
#include <inviwo/core/io/imageloader.h>

namespace inviwo {

    ImageDisk::ImageDisk()
        : ImageRepresentation(uvec2(128,128), DataVec4UINT8()), DiskRepresentation(){}

	ImageDisk::ImageDisk(std::string url)
        : ImageRepresentation(), DiskRepresentation(url){
        initialize();
	}

    ImageDisk::~ImageDisk() {
    }

    void ImageDisk::initialize(){
        if(hasSourceFile()){
            dimensions_ = ImageLoader::imageDimensions(getSourceFile());
        }
    }

    void* ImageDisk::loadFileData(){
        if(hasSourceFile())
            return ImageLoader::loadImageToData(getSourceFile());

        return NULL;
    }

    void ImageDisk::deinitialize() {}

    DataRepresentation* ImageDisk::clone() {
        return new ImageDisk(getSourceFile());
    }

} // namespace
