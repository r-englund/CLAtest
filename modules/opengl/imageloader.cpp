#include "imageloader.h"
/**
* Internal function to load a image 
* @param filename the name of the file to be loaded
* @param bitmap the bitmap to store the loaded image.
* @see loadImageToTexture()
* @return if the load was successful.
*/

bool ImageLoader::loader_initialized = false;

bvec2 ImageLoader::imageDimensions(std::string filename){
    if(!loader_initialized)
	{
		loader_initialized = true;
		FreeImage_Initialise(1);
	}

    FIBITMAP *bitmap = new FIBITMAP(); 
    readInImage(filename, &bitmap);
	int width = FreeImage_GetWidth(bitmap);
	int height = FreeImage_GetHeight(bitmap);
    return bvec2(width, height);
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

/**
* Internal function that finds largest power of two smaller then the input value
* @param number is a number that is larger then zero.
*/
void ImageLoader::powerOfTwo(int& number){
	int power = 1;
	while ( power < number )
		power <<= 1;
	
	number = power;
}

/**
* Internal function to load a bitmap to a texture
* @param bitmap is where the loaded image should be loaded.
* @return texture that has been loaded from the bitmap
*/
Texture2D* ImageLoader::loadImageToTexture(FIBITMAP *bitmap){
	int width = FreeImage_GetWidth(bitmap);
	int height = FreeImage_GetHeight(bitmap);

	FIBITMAP *bitmap2 = FreeImage_Allocate(width, height, 32);
    FreeImage_Paste(bitmap2, bitmap, 0, 0, 255);

	GLubyte *textureData = new GLubyte[4 * width * height];
	GLubyte *pixelValues = (GLubyte*)FreeImage_GetBits(bitmap2);


	//Fill in the texture, a shift is needed to convert from BGRA to RGBA
	for(int i = 0; i < width * height; i++){
		textureData[i * 4 + 0] = pixelValues[i * 4 + 2];
		textureData[i * 4 + 1] = pixelValues[i * 4 + 1];
		textureData[i * 4 + 2] = pixelValues[i * 4 + 0];
		textureData[i * 4 + 3] = pixelValues[i * 4 + 3];
	}
	//create new texture with correct dimensions
	Texture2D* texture = new Texture2D(uvec2(width, height), GL_RGBA, GL_RGBA8, GL_UNSIGNED_BYTE, GL_NEAREST);

	texture->setTexels(textureData);
	texture->setWidth(width);
	texture->setHeight(height);

	texture->upload();

	return texture;

}

/**
* Loads an image with specified filename to texture if the filename points to a file that
* is supported by FreeImage. A new instance of the texture will be created with correct 
* dimensions. The texture will be bound to the currently activated texture.
* @param filename is the name of the file that is to be loaded.
* @return texture that has been loaded. Returns NULL if no texture was loaded.
*/
Texture2D* ImageLoader::loadImage(std::string filename){
    initLoader();
	FIBITMAP *bitmap = new FIBITMAP(); 
	//If the file is read as it should
	if(readInImage(filename, &bitmap)){
		return loadImageToTexture(bitmap);
	}
	return NULL;
}

uint8_t* ImageLoader::imageToBitmap(FIBITMAP *bitmap){
    int width = FreeImage_GetWidth(bitmap);
	int height = FreeImage_GetHeight(bitmap);
	
    FIBITMAP *bitmap2 = FreeImage_Allocate(width, height, 32);
    FreeImage_Paste(bitmap2, bitmap, 0, 0, 255);

	GLubyte *pixelValues = (GLubyte*)FreeImage_GetBits(bitmap2);
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

/**
* Loads an image to bitmap.
* @param filename is the file that is to be loaded
* @return bitmap
*/
uint8_t* ImageLoader::loadImageToData(std::string filename){
    initLoader();
    FIBITMAP *freeimage_bitmap = new FIBITMAP();
    if(readInImage(filename, &freeimage_bitmap)){
        return imageToBitmap(freeimage_bitmap);
    }
    return 0;
}

/**
* Initializes freeimage if needed.
*/
void ImageLoader::initLoader(){
    if(!loader_initialized){
		loader_initialized = true;
		FreeImage_Initialise(1);
	}
}