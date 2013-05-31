#include <inviwo/core/datastructures/imagedisk.h>
#include <inviwo/core/io/imageloader.h>

namespace inviwo {

    ImageDisk::ImageDisk()
        : ImageRepresentation(uvec2(256,256), DataVec4UINT8()), DiskRepresentation(){}

	ImageDisk::ImageDisk(std::string url)
        : ImageRepresentation(uvec2(256,256), DataVec4UINT8()), DiskRepresentation(url){
        initialize();
	}

    ImageDisk::~ImageDisk() {
    }

    void ImageDisk::initialize(){
        if (hasSourceFile()){
            dimensions_ = ImageLoader::imageDimensions(getSourceFile());
        }
    }

    void* ImageDisk::loadFileData(){
        if (hasSourceFile())
            
            return ImageLoader::loadImageToData(getSourceFile());

        return NULL;
    }

    void ImageDisk::deinitialize() {}

    DataRepresentation* ImageDisk::clone() const {
        return new ImageDisk(getSourceFile());
    }

} // namespace
