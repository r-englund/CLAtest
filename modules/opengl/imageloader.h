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
	static Texture2D* loadImage(std::string filename);
private:
	static bool readInImage(std::string filename, FIBITMAP **bitmap);

	static void powerOfTwo(int& number);

	static Texture2D* loadImageToTexture(FIBITMAP *bitmap);

	static bool loader_initialized;
};
#endif