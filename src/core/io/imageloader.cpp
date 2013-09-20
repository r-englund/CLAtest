#include <inviwo/core/io/imageloader.h>

bool ImageLoader::loader_initialized = false;

uvec2 ImageLoader::imageDimensions(std::string filename){
    initLoader();
    FIBITMAP *bitmap = new FIBITMAP(); 
    readInImage(filename, &bitmap);
    if (bitmap != NULL){
	    unsigned int width = FreeImage_GetWidth(bitmap);
	    unsigned int height = FreeImage_GetHeight(bitmap);
        return uvec2(width, height);
    }
    return uvec2(512, 512);
}

template<typename T>
T* ImageLoader::switchChannels(const T *inputImage, uvec2 dim){
    T* result = new T[4 * dim.x * dim.y];
    for(unsigned int i = 0; i < dim.x * dim.y; i++){
        result[i * 4 + 0] = inputImage[i * 4 + 2];
        result[i * 4 + 1] = inputImage[i * 4 + 1];
        result[i * 4 + 2] = inputImage[i * 4 + 0];
        result[i * 4 + 3] = inputImage[i * 4 + 3];
    }
    return result;
}

template<typename T>
FIBITMAP* ImageLoader::convertToBitmap(T *data, uvec2 dim, size_t bitsPerPixel){
    FIBITMAP *dib = FreeImage_Allocate(dim.x, dim.y, static_cast<int>(bitsPerPixel), FI_RGBA_RED_MASK,
                                        FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK);

    unsigned int bytespp = FreeImage_GetLine(dib) / FreeImage_GetWidth(dib);
    T *bits = (T*)FreeImage_GetBits(dib);
    for(unsigned i = 0; i < dim.x * dim.y; i++){
        for(unsigned j = 0; j < bytespp; j++){
            bits[i * bytespp + j] = data[i * bytespp + j];
        }
    }
    return dib;
}


template<typename T>
FIBITMAP* ImageLoader::handleConvertions(const T *data, size_t bitsPerPixel, uvec2 dim){
    T* dataswitch = switchChannels<T>(data, dim);
    FIBITMAP *bitmap = convertToBitmap<T>(dataswitch, dim, bitsPerPixel);
    return bitmap;
    //return NULL;
}


FIBITMAP* ImageLoader::convertToByte(const ImageRAM* inputImage, uvec2 dim, size_t bitsPerPixel){
    switch (inputImage->getDataFormatId()) {
    case inviwo::UINT8:
        return handleConvertions<DataUINT8::type>(static_cast<const DataUINT8::type*>(inputImage->getData()), bitsPerPixel, dim);
    case Vec4UINT8:{
        //Convert vector to uint8
        const DataVec4UINT8::type *data = static_cast<const DataVec4UINT8::type*>(inputImage->getData());

        DataUINT8::type *newData = new DataUINT8::type[4 * dim.x * dim.y];
        for(unsigned i = 0; i < dim.x * dim.y; i++){
            newData[i * 4 + 0] = data[i].r;
            newData[i * 4 + 1] = data[i].g;
            newData[i * 4 + 2] = data[i].b;
            newData[i * 4 + 3] = data[i].a;
        }
        return handleConvertions<DataUINT8::type>(newData, bitsPerPixel, dim);
        break;
        }
    default: 
        //LogErrorS("ImageExporter", "Cannot save format:" + (inputImage->getDataFormat()->getString()) );
        break;
    }
    return NULL;
}


void ImageLoader::saveImage(const char* filename, const Image* inputImage)
{
    initLoader();
    FREE_IMAGE_FORMAT imageFormat = FreeImage_GetFIFFromFilename(filename);
    
    if (imageFormat != FIF_UNKNOWN && inputImage != NULL){
	    const ImageRAM *imageRam = inputImage->getRepresentation<ImageRAM>();

        assert(imageRam != NULL);

        uvec2 dim = inputImage->getDimension();
    
        const DataFormatBase* dataformat = imageRam->getDataFormat();
        size_t bitsPerPixel = dataformat->getBitsAllocated();
        assert(bitsPerPixel != 0);

        FIBITMAP* bitmap = convertToByte(imageRam, dim, bitsPerPixel);
 
        FreeImage_Save(imageFormat, bitmap, filename, static_cast<int>(bitsPerPixel));

    }
    else{
        //Unknown file ending
    }
}


bool ImageLoader::readInImage(std::string filename, FIBITMAP **bitmap){

	const char* file_name_char = (char*)(filename.c_str());
	FREE_IMAGE_FORMAT imageFormat = FIF_UNKNOWN;	

	//Get file format of input file
	imageFormat = FreeImage_GetFileType(file_name_char, 10);
	
	if (imageFormat == FIF_UNKNOWN){
		imageFormat = FreeImage_GetFIFFromFilename(file_name_char);

		//Raw image files conflicting with raw 3d volumes.
		if (imageFormat == FIF_RAW){
			return false;
		}
	}

	//Load image if format is supported
	if (imageFormat != FIF_UNKNOWN){
		*bitmap = FreeImage_Load(imageFormat, file_name_char, 0);
	}
	//Return if format was found.
	return (imageFormat != FIF_UNKNOWN);
}


template<typename T>
T* ImageLoader::fiBitmapToDataArray(FIBITMAP *bitmap){
    int width = FreeImage_GetWidth(bitmap);
	int height = FreeImage_GetHeight(bitmap);
	uvec2 dim(width, height);
    FIBITMAP *bitmap2 = FreeImage_Allocate(width, height, 32);
    FreeImage_Paste(bitmap2, bitmap, 0, 0, 255);

	T *pixelValues = (T*)FreeImage_GetBits(bitmap2);

    ////Fill in the texture, a shift is needed to convert from BGRA to RGBA
    T* data = switchChannels<T>(pixelValues, dim);
	
    return data;
}


void* ImageLoader::loadImageToData(std::string filename){
    initLoader();
    FIBITMAP *freeimage_bitmap = new FIBITMAP();
    if (readInImage(filename, &freeimage_bitmap)){
        return (void*)fiBitmapToDataArray<DataUINT8::type>(freeimage_bitmap);
    }
    return NULL;
}

template<typename T>
T* ImageLoader::fiBitmapToDataArrayAndRescale(FIBITMAP *bitmap, int dst_width, int dst_height){
    int width = FreeImage_GetWidth(bitmap);
    int height = FreeImage_GetHeight(bitmap);
    uvec2 dim(width, height);
    uvec2 dst_dim(dst_width, dst_height);

    if (dim==dst_dim)
        return fiBitmapToDataArray<T>(bitmap);

    FIBITMAP *bitmap2 = FreeImage_Allocate(width, height, 32);
    FreeImage_Paste(bitmap2, bitmap, 0, 0, 255);

    bitmap2 = FreeImage_Rescale(bitmap2, dst_width, dst_height, FILTER_BILINEAR);

    T *pixelValues = (T*)FreeImage_GetBits(bitmap2);

    ////Fill in the texture, a shift is needed to convert from BGRA to RGBA
    T* data = switchChannels<T>(pixelValues, dst_dim);

    return data;
}

void* ImageLoader::loadImageToDataAndRescale(std::string filename, int dst_width, int dst_height){
    initLoader();
    FIBITMAP *freeimage_bitmap = new FIBITMAP();
    if (readInImage(filename, &freeimage_bitmap)){
        return (void*)fiBitmapToDataArrayAndRescale<DataUINT8::type>(freeimage_bitmap, dst_width, dst_height);
    }
    return NULL;
}

void* ImageLoader::rescaleImage(Image* srcImage, int dst_width, int dst_height) {
    const ImageRAM *imageRam = srcImage->getRepresentation<ImageRAM>();
    return rescaleImageRAM(const_cast<ImageRAM*>(imageRam), dst_width, dst_height);
}

void* ImageLoader::rescaleImageRAM(ImageRAM* srcImageRam, int dst_width, int dst_height) {
    ivwAssert(srcImageRam!=NULL, "ImageRAM representation does not exist.");

    initLoader();
    uvec2 dim = srcImageRam->getDimensions();

    const DataFormatBase* dataformat = srcImageRam->getDataFormat();
    size_t bitsPerPixel = dataformat->getBitsAllocated();
    ivwAssert(bitsPerPixel!=0, "Invalid data format.");

    FIBITMAP* bitmap = convertToByte(srcImageRam, dim, bitsPerPixel);
    if (bitmap) {
        void* rawData = (void*)fiBitmapToDataArrayAndRescale<DataUINT8::type>(bitmap, dst_width, dst_height);
        ivwAssert(rawData!=NULL, "Unable to rescale image ram representation.");
        return rawData;
    }
    return NULL;
}

void ImageLoader::initLoader(){
    if (!loader_initialized){
		loader_initialized = true;
		FreeImage_Initialise(1);
	}
}