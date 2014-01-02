/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#include <inviwo/core/io/imageloader.h>

bool ImageLoader::loader_initialized = false;

void ImageLoader::saveImage(const char* filename, const Image* inputImage) {
    initLoader();
    FREE_IMAGE_FORMAT imageFormat = FreeImage_GetFIFFromFilename(filename);
    
    if (imageFormat != FIF_UNKNOWN && inputImage != NULL){
	    const ImageRAM *imageRam = inputImage->getRepresentation<ImageRAM>();

        assert(imageRam != NULL);

        FIBITMAP* bitmap = createBitmapFromData(imageRam);
 
        FreeImage_Save(imageFormat, bitmap, filename, static_cast<int>(imageRam->getDataFormat()->getBitsAllocated()));

        FreeImage_Unload(bitmap);
    }
    else{
        //Unknown file ending
    }
}

bool ImageLoader::readInImage(std::string filename, FIBITMAP** bitmap){
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

bool ImageLoader::isValidImageFile(std::string filename) {
    initLoader();
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
    
    //Return if format was found.
    return (imageFormat != FIF_UNKNOWN);    
}


template<typename T>
T* ImageLoader::fiBitmapToDataArray(FIBITMAP* bitmap){
    int width = FreeImage_GetWidth(bitmap);
	int height = FreeImage_GetHeight(bitmap);
	uvec2 dim(width, height);
    FIBITMAP *bitmapNEW = FreeImage_Allocate(width, height, 32);
    FreeImage_Paste(bitmapNEW, bitmap, 0, 0, 255);

    switchChannels(bitmapNEW, dim);

	T *pixelValues = (T*)FreeImage_GetBits(bitmapNEW);
	
    return pixelValues;
}

void* ImageLoader::loadImageToData(std::string filename){
    initLoader();
    FIBITMAP *bitmap = new FIBITMAP();
    void* out = NULL;
    if (readInImage(filename, &bitmap)){
        out = (void*)fiBitmapToDataArray<DataUINT8::type>(bitmap);
    }
    return out;
}

void* ImageLoader::loadImageToDataAndRescale(std::string filename, int dst_width, int dst_height){
    initLoader();
    FIBITMAP* bitmap = new FIBITMAP();
    void* out = NULL;
    if (readInImage(filename, &bitmap)){
        out = (void*)fiBitmapToDataArrayAndRescale<DataUINT8::type>(bitmap, dst_width, dst_height);
    }
    FreeImage_Unload(bitmap);
    return out;
}

template<typename T>
T* ImageLoader::fiBitmapToDataArrayAndRescale(FIBITMAP* bitmap, int dst_width, int dst_height){
    int width = FreeImage_GetWidth(bitmap);
    int height = FreeImage_GetHeight(bitmap);
    uvec2 dim(width, height);
    uvec2 dst_dim(dst_width, dst_height);

    if (dim==dst_dim)
        return fiBitmapToDataArray<T>(bitmap);

    FIBITMAP *bitmap2 = FreeImage_Allocate(width, height, 32);
    FreeImage_Paste(bitmap2, bitmap, 0, 0, 255);

    FIBITMAP* bitmapNEW = FreeImage_Rescale(bitmap2, dst_width, dst_height, FILTER_BILINEAR);
    FreeImage_Unload(bitmap2);

    switchChannels(bitmapNEW, dim);

    T *pixelValues = (T*)FreeImage_GetBits(bitmapNEW);

    return pixelValues;
}

void* ImageLoader::rescaleImage(Image* srcImage, int dst_width, int dst_height) {
    const ImageRAM *imageRam = srcImage->getRepresentation<ImageRAM>();
    return rescaleImageRAM(const_cast<ImageRAM*>(imageRam), dst_width, dst_height);
}

void* ImageLoader::rescaleImageRAM(ImageRAM* srcImageRam, int dst_width, int dst_height) {
    ivwAssert(srcImageRam!=NULL, "ImageRAM representation does not exist.");

    initLoader();  

    void* rawData = NULL;
    FIBITMAP* bitmap = NULL;
    switch (srcImageRam->getDataFormatId())
    {
    case NOT_SPECIALIZED:
        LogErrorCustom("rescaleImageRAM", "Invalid format");
        rawData = NULL;
        break;
#define DataFormatIdMacro(i) case inviwo::i: bitmap = handleBitmapCreations<Data##i::type>(static_cast<const Data##i::type*>(srcImageRam->getData()), srcImageRam->getDimensions(), Data##i::bitsAllocated()); \
    rawData = (void*)fiBitmapToDataArrayAndRescale<Data##i::type>(bitmap, dst_width, dst_height); break;
#include <inviwo/core/util/formatsdefinefunc.h>
    default:
        LogErrorCustom("rescaleImageRAM", "Invalid format or not implemented");
        rawData = NULL;
        break;
    }
    FreeImage_Unload(bitmap);
    ivwAssert(rawData!=NULL, "Unable to rescale image ram representation.");
    return rawData;
}

void ImageLoader::switchChannels(FIBITMAP* bitmap, uvec2 dim){
    BYTE* result = FreeImage_GetBits(bitmap);
    BYTE tmp;
    for(unsigned int i = 0; i < dim.x * dim.y; i++){
        tmp = result[i * 4 + 0];
        result[i * 4 + 0] = result[i * 4 + 2];
        result[i * 4 + 2] = tmp;
    }
}

template<typename T>
FIBITMAP* ImageLoader::createBitmapFromData(const T* data, uvec2 dim, size_t bitsPerPixel){
    FIBITMAP* dib = FreeImage_Allocate(dim.x, dim.y, static_cast<int>(bitsPerPixel), FI_RGBA_RED_MASK,
        FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK);

    unsigned int bytespp = FreeImage_GetLine(dib) / FreeImage_GetWidth(dib);
    T* bits = (T*)FreeImage_GetBits(dib);
    memcpy(bits, data, dim.x*dim.y*bytespp);
    return dib;
}

template<typename T>
FIBITMAP* ImageLoader::handleBitmapCreations(const T* data, uvec2 dim, size_t bitsPerPixel){
    FIBITMAP *bitmap = createBitmapFromData<T>(data, dim, bitsPerPixel);
    switchChannels(bitmap, dim);
    return bitmap;
}

FIBITMAP* ImageLoader::createBitmapFromData(const ImageRAM* inputImage){
    switch (inputImage->getDataFormatId())
    {
    case NOT_SPECIALIZED:
        LogErrorCustom("createBitmapFromData", "Invalid format");
        return NULL;
#define DataFormatIdMacro(i) case inviwo::i: return handleBitmapCreations<Data##i::type>(static_cast<const Data##i::type*>(inputImage->getData()), inputImage->getDimensions(), Data##i::bitsAllocated());
#include <inviwo/core/util/formatsdefinefunc.h>
    default:
        LogErrorCustom("createBitmapFromData", "Invalid format or not implemented");
        break;
    }
    return NULL;
}

void ImageLoader::initLoader(){
    if (!loader_initialized){
		loader_initialized = true;
		FreeImage_Initialise(1);
	}
}