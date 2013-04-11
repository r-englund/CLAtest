#include <inviwo/core/io/imageloader.h>

bool ImageLoader::loader_initialized = false;

uvec2 ImageLoader::imageDimensions(std::string filename){
    initLoader();
    FIBITMAP *bitmap = new FIBITMAP(); 
    readInImage(filename, &bitmap);
	unsigned int width = FreeImage_GetWidth(bitmap);
	unsigned int height = FreeImage_GetHeight(bitmap);
    return uvec2(width, height);
}

bool ImageLoader::readInImage(std::string filename, FIBITMAP **bitmap){

	const char* file_name_char = (char*)(filename.c_str());
	FREE_IMAGE_FORMAT imageFormat = FIF_UNKNOWN;	

	//Get file format of input file
	imageFormat = FreeImage_GetFileType(file_name_char, 10);
	
	if(imageFormat == FIF_UNKNOWN){
		imageFormat = FreeImage_GetFIFFromFilename(file_name_char);

		//Raw image files conflicting with raw 3d volumes.
		if(imageFormat == FIF_RAW){
			return false;
		}
	}

	//Load image if format is supported
	if(imageFormat != FIF_UNKNOWN){
		*bitmap = FreeImage_Load(imageFormat, file_name_char, 0);
	}
	//Return if format was found.
	return (imageFormat != FIF_UNKNOWN);
}

uint8_t* ImageLoader::imageToBitmap(FIBITMAP *bitmap){
    int width = FreeImage_GetWidth(bitmap);
	int height = FreeImage_GetHeight(bitmap);
	
    FIBITMAP *bitmap2 = FreeImage_Allocate(width, height, 32);
    FreeImage_Paste(bitmap2, bitmap, 0, 0, 255);

	BYTE *pixelValues = FreeImage_GetBits(bitmap2);
    uint8_t* data = new uint8_t[4 * width * height];

	//Fill in the texture, a shift is needed to convert from BGRA to RGBA
	for(int i = 0; i < width * height; i++){
		data[i * 4 + 0] = static_cast<uint8_t>(pixelValues[i * 4 + 2]);
		data[i * 4 + 1] = static_cast<uint8_t>(pixelValues[i * 4 + 1]);
		data[i * 4 + 2] = static_cast<uint8_t>(pixelValues[i * 4 + 0]);
		data[i * 4 + 3] = static_cast<uint8_t>(pixelValues[i * 4 + 3]);
	}
    return data;
}

uint8_t* ImageLoader::loadImageToData(std::string filename){
    initLoader();
    FIBITMAP *freeimage_bitmap = new FIBITMAP();
    if(readInImage(filename, &freeimage_bitmap)){
        return imageToBitmap(freeimage_bitmap);
    }
    return 0;
}

void ImageLoader::initLoader(){
    if(!loader_initialized){
		loader_initialized = true;
		FreeImage_Initialise(1);
	}
}