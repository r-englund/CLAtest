#ifndef IVW_IMAGELOADER_H
#define IVW_IMAGELOADER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <ext/freeimage/FreeImage.h>

#include <inviwo/core/datastructures/image/image.h>

#include <inviwo/core/util/formats.h>
#include <inviwo/core/datastructures/image/imageram.h>
using namespace inviwo;

class IVW_CORE_API ImageLoader{
public:
	ImageLoader(){};
	~ImageLoader(){
		FreeImage_DeInitialise();
	}
    /**
    * Loads an image to bitmap.
    * @param filename is the file that is to be loaded
    * @return bitmap
    */ 
     static void* loadImageToData(std::string filename);
    
    /**
    * Saves an image to a specified filename.
    * @param filename the path including name to file that is to be stored.
    * @param inputImage specifies the image that is to be saved.
    **/
    static void saveImage(const char* filename, const Image* inputImage);

    /**
    * Calculates image dimensions.
    * @param filename is the image.
    * @return dimensions of image
    */
    static uvec2 imageDimensions(std::string filename);
private:
    /**
    * Internal function to load a image 
    * @param filename the name of the file to be loaded
    * @param bitmap the bitmap to store the loaded image.
    * @return if the load was successful.
    */
	static bool readInImage(std::string filename, FIBITMAP **bitmap);

    /**
    * Initializes freeimage if needed.
    **/
    static void initLoader();

    /**
    * Converts image to byte array.
    * @param inputImage is the image that is to be converted.
    * @param dim is the dimensions of the image.
    * @return the converted image.
    */
    static FIBITMAP* convertToByte(const ImageRAM* inputImage, uvec2 dim, size_t bitsPerPixel);

template<typename T>
    static FIBITMAP* handleConvertions(const T *data, size_t bitsPerPixel, uvec2 dim);

template<typename T>
    static FIBITMAP* convertToBitmap(T *data, uvec2 dim, size_t bitsPerPixel);

    /**
    * Switch red and blue channels in the image.
    * @param inputImage is the image that is switching channels.
    * @param dim is the dimension of the image.
    * @return the new image.
    */
template<typename T>
    static T* switchChannels(const T *inputImage, uvec2 dim);

    /**
    * Converts an image from freeimage format to regular int.
    * @param bitmap is the bitmap to convert
    * @return the converted bitmap.
    **/
template<typename T>
    static T* fiBitmapToDataArray(FIBITMAP *bitmap);

	static bool loader_initialized;
};
#endif // IVW_IMAGELOADER_H