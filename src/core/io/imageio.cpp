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

#include <inviwo/core/io/imageio.h>

bool ImageIO::loader_initialized = false;

inline DataFormatId getDataFormatFromBitmap(FIBITMAP* bitmap){
    FREE_IMAGE_TYPE type = FreeImage_GetImageType(bitmap);
    unsigned int bpp = FreeImage_GetBPP(bitmap);

    switch(type){
        case FIT_UNKNOWN:
            break;
        case FIT_BITMAP:
            switch(bpp){
                case 1:
                case 4:
                case 8:
                    return inviwo::UINT8;
                case 16:
                    return inviwo::Vec2UINT8;
                case 24:
                case 32:
                    return inviwo::Vec4UINT8;
            }
            break;
        case FIT_UINT16:
            return inviwo::UINT16;
        case FIT_INT16:
            return inviwo::INT16;
        case FIT_UINT32:
            return inviwo::UINT32;
        case FIT_INT32:
            return inviwo::INT32;
        case FIT_FLOAT:
            return inviwo::FLOAT32;
        case FIT_DOUBLE:
            return inviwo::FLOAT64;
        case FIT_COMPLEX:
            return inviwo::Vec2FLOAT64;
        case FIT_RGB16:
            return inviwo::Vec3UINT16;
        case FIT_RGBA16:
            return inviwo::Vec4UINT16;
        case FIT_RGBF:
            return inviwo::Vec3FLOAT32;
        case FIT_RGBAF:
            return inviwo::Vec4FLOAT32;
        default:
            break;
    }

    return inviwo::NOT_SPECIALIZED;
}

void ImageIO::saveImage(const char* filename, const Image* inputImage) {
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

bool ImageIO::readInImage(std::string filename, FIBITMAP** bitmap){
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

bool ImageIO::isValidImageFile(std::string filename) {
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

DataFormatId ImageIO::loadImageToData(void* data, std::string filename){
    initLoader();
    FIBITMAP *bitmap = new FIBITMAP();
    DataFormatId formatId = NOT_SPECIALIZED;
    if (readInImage(filename, &bitmap)){
        formatId = getDataFormatFromBitmap(bitmap);
        switch (formatId)
        {
        case NOT_SPECIALIZED:
            LogErrorCustom("loadImageToData", "Invalid format");
            break;
#define DataFormatIdMacro(i) case inviwo::i: fiBitmapToDataArray<Data##i::type>(data, bitmap, Data##i::bitsAllocated(), Data##i::components()); break;
#include <inviwo/core/util/formatsdefinefunc.h>
        default:
            LogErrorCustom("loadImageToData", "Invalid format or not implemented");
            break;
        }
    }
    FreeImage_Unload(bitmap);
    return formatId;
}

DataFormatId ImageIO::loadImageToDataAndRescale(void* data, std::string filename, int dst_width, int dst_height){
    initLoader();
    FIBITMAP* bitmap = new FIBITMAP();
    DataFormatId formatId = NOT_SPECIALIZED;
    if (readInImage(filename, &bitmap)){
        formatId = getDataFormatFromBitmap(bitmap);
        switch (formatId)
        {
        case NOT_SPECIALIZED:
            LogErrorCustom("loadImageToDataAndRescale", "Invalid format");
            break;
#define DataFormatIdMacro(i) case inviwo::i: fiBitmapToDataArrayAndRescale<Data##i::type>(data, bitmap, dst_width, dst_height, Data##i::bitsAllocated(), Data##i::components()); break;
#include <inviwo/core/util/formatsdefinefunc.h>
        default:
            LogErrorCustom("loadImageToDataAndRescale", "Invalid format or not implemented");
            break;
        }
    }
    FreeImage_Unload(bitmap);
    return formatId;
}

void* ImageIO::rescaleImage(Image* srcImage, int dst_width, int dst_height) {
    const ImageRAM *imageRam = srcImage->getRepresentation<ImageRAM>();
    return rescaleImageRAM(const_cast<ImageRAM*>(imageRam), dst_width, dst_height);
}

void* ImageIO::rescaleImageRAM(ImageRAM* srcImageRam, int dst_width, int dst_height) {
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
#define DataFormatIdMacro(i) case inviwo::i: bitmap = handleBitmapCreations<Data##i::type>(static_cast<const Data##i::type*>(srcImageRam->getData()), srcImageRam->getDimensions(), Data##i::bitsAllocated(), Data##i::components()); \
    fiBitmapToDataArrayAndRescale<Data##i::type>(static_cast<Data##i::type*>(rawData), bitmap, dst_width, dst_height, Data##i::bitsAllocated(), Data##i::components()); break;
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

void ImageIO::switchChannels(FIBITMAP* bitmap, uvec2 dim, int channels){
    if(channels > 2){
        unsigned int c = static_cast<unsigned int>(channels);
        BYTE* result = FreeImage_GetBits(bitmap);
        BYTE tmp;
        for(unsigned int i = 0; i < dim.x * dim.y; i++){
            tmp = result[i * c + 0];
            result[i * c + 0] = result[i * c + 2];
            result[i * c + 2] = tmp;
        }
    }
}

FIBITMAP* ImageIO::allocateBitmap(int width, int height, size_t bitsPerPixel, int channels){
    unsigned int rMask = FI_RGBA_RED_MASK;
    unsigned int gMask = FI_RGBA_GREEN_MASK;
    unsigned int bMask = FI_RGBA_BLUE_MASK;

    if(channels < 3){
        bMask = 0;
    }
    else if(channels < 2){
        gMask = 0;
        bMask = 0;
    }
    else if(channels < 1){
        rMask = 0;
        gMask = 0;
        bMask = 0;
    }

    return FreeImage_Allocate(width, height, static_cast<int>(bitsPerPixel), rMask, gMask, bMask);
}

template<typename T>
FIBITMAP* ImageIO::createBitmapFromData(const T* data, uvec2 dim, size_t bitsPerPixel, int channels){
    FIBITMAP* dib = allocateBitmap(dim.x, dim.y, bitsPerPixel, channels);
    unsigned int bytespp = FreeImage_GetLine(dib) / FreeImage_GetWidth(dib);
    T* bits = (T*)FreeImage_GetBits(dib);
    memcpy(bits, data, dim.x*dim.y*bytespp);
    return dib;
}

template<typename T>
FIBITMAP* ImageIO::handleBitmapCreations(const T* data, uvec2 dim, size_t bitsPerPixel, int channels){
    FIBITMAP *bitmap = createBitmapFromData<T>(data, dim, bitsPerPixel, channels);
    switchChannels(bitmap, dim, channels);
    return bitmap;
}

FIBITMAP* ImageIO::createBitmapFromData(const ImageRAM* inputImage){
    switch (inputImage->getDataFormatId())
    {
    case NOT_SPECIALIZED:
        LogErrorCustom("createBitmapFromData", "Invalid format");
        return NULL;
#define DataFormatIdMacro(i) case inviwo::i: return handleBitmapCreations<Data##i::type>(static_cast<const Data##i::type*>(inputImage->getData()), inputImage->getDimensions(), Data##i::bitsAllocated(), Data##i::components());
#include <inviwo/core/util/formatsdefinefunc.h>
    default:
        LogErrorCustom("createBitmapFromData", "Invalid format or not implemented");
        break;
    }
    return NULL;
}

template<typename T>
void ImageIO::fiBitmapToDataArray(void* dst, FIBITMAP* bitmap, size_t bitsPerPixel, int channels){
    int width = FreeImage_GetWidth(bitmap);
    int height = FreeImage_GetHeight(bitmap);
    uvec2 dim(width, height);
    FIBITMAP* bitmapNEW = allocateBitmap(width, height, bitsPerPixel, channels);
    FreeImage_Paste(bitmapNEW, bitmap, 0, 0, 255);

    switchChannels(bitmapNEW, dim, channels);

    void* pixelValues = static_cast<void*>(FreeImage_GetBits(bitmapNEW));

    if(!dst){
        T* dstAlloc = new T[dim.x*dim.y];
        dst = static_cast<void*>(dstAlloc);
    }

    memcpy(dst, pixelValues, dim.x*dim.y*sizeof(T));
    FreeImage_Unload(bitmapNEW);
}

template<typename T>
void ImageIO::fiBitmapToDataArrayAndRescale(void* dst, FIBITMAP* bitmap, int dst_width, int dst_height, size_t bitsPerPixel, int channels){
    int width = FreeImage_GetWidth(bitmap);
    int height = FreeImage_GetHeight(bitmap);
    uvec2 dim(width, height);
    uvec2 dst_dim(dst_width, dst_height);

    if (dim==dst_dim)
        return fiBitmapToDataArray<T>(dst, bitmap, bitsPerPixel, channels);

    FIBITMAP *bitmap2 = allocateBitmap(width, height, bitsPerPixel, channels);
    FreeImage_Paste(bitmap2, bitmap, 0, 0, 255);

    FIBITMAP* bitmapNEW = FreeImage_Rescale(bitmap2, dst_width, dst_height, FILTER_BILINEAR);
    FreeImage_Unload(bitmap2);

    switchChannels(bitmapNEW, dst_dim, channels);

    void* pixelValues = static_cast<void*>(FreeImage_GetBits(bitmapNEW));

    if(!dst){
        T* dstAlloc = new T[dst_dim.x*dst_dim.y];
        dst = static_cast<void*>(dstAlloc);
    }

    memcpy(dst, pixelValues, dst_dim.x*dst_dim.y*sizeof(T));
    FreeImage_Unload(bitmapNEW);
}

void ImageIO::initLoader(){
    if (!loader_initialized){
		loader_initialized = true;
		FreeImage_Initialise(1);
	}
}