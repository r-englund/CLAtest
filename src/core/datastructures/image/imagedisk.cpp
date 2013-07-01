#include <inviwo/core/datastructures/image/imagedisk.h>
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

    void* ImageDisk::loadFileData() const {
        if (hasSourceFile())            
            return ImageLoader::loadImageToData(getSourceFile());

        return NULL;
    }

    void* ImageDisk::loadFileDataAndRescale(uvec2 dst_dimesion) const {
        if (hasSourceFile())
            return ImageLoader::loadImageToDataAndRescale(getSourceFile(), dst_dimesion.x, dst_dimesion.y);

        return NULL;
    }

    void ImageDisk::deinitialize() {}

    void ImageDisk::resize(uvec2 dimensions){ 
        //Preserve the original dimension.        
        IVW_UNUSED_PARAM(dimensions); 
    } 

    DataRepresentation* ImageDisk::clone() const {
        return new ImageDisk(getSourceFile());
    }

} // namespace
