#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include "modules/opengl/openglmoduledefine.h"
#include "inviwo/core/inviwo.h"
#include "glwrap/texture2d.h"
#include "ext/freeimage/FreeImage.h"

using namespace inviwo;

class ImageLoader{
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
    static uint8_t* loadImageToData(std::string filename);

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
    * Converts an image from freeimage format to regular int.
    * @param bitmap is the bitmap to convert
    * @return the converted bitmap.
    **/
    static uint8_t* imageToBitmap(FIBITMAP *bitmap);

	static bool loader_initialized;
};
#endif