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

inline FREE_IMAGE_TYPE getFreeImageFormatFromDataFormat(DataFormatId formatId){
    switch(formatId){
        case inviwo::NOT_SPECIALIZED:
            break;
        case inviwo::UINT8:
        case inviwo::Vec2UINT8:
        case inviwo::Vec3UINT8:
        case inviwo::Vec4UINT8:
            return FIT_BITMAP;
        case inviwo::UINT16:
            return FIT_UINT16;
        case inviwo::INT16:
            return FIT_INT16;
        case inviwo::UINT32:
            return FIT_UINT32;
        case inviwo::INT32:
            return FIT_INT32;
        case inviwo::FLOAT32:
            return FIT_FLOAT;
        case inviwo::FLOAT64:
            return FIT_DOUBLE;
        case inviwo::Vec2FLOAT64:
            return FIT_COMPLEX;
        case inviwo::Vec3UINT16:
            return FIT_RGB16;
        case inviwo::Vec4UINT16:
            return FIT_RGBA16;
        case inviwo::Vec3FLOAT32:
            return FIT_RGBF;
        case inviwo::Vec4FLOAT32:
            return FIT_RGBAF;
        default:
            break;
    }

    return FIT_UNKNOWN;
}

void ImageIO::saveLayer(const char* filename, const Layer* inputLayer) {
    initLoader();
    FREE_IMAGE_FORMAT imageFormat = FreeImage_GetFIFFromFilename(filename);
    
    if (imageFormat != FIF_UNKNOWN && inputLayer != NULL){
	    const LayerRAM *imageRam = inputLayer->getRepresentation<LayerRAM>();

        assert(imageRam != NULL);

        FIBITMAP* bitmap = createBitmapFromData(imageRam);

        FIBITMAP* bitmapStandard = FreeImage_ConvertToStandardType(bitmap);
 
        FreeImage_Save(imageFormat, bitmapStandard, filename, static_cast<int>(imageRam->getDataFormat()->getBitsAllocated()));

        FreeImage_Unload(bitmapStandard);
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

void* ImageIO::loadImageToData(void* data, std::string filename, uvec2& out_dim, DataFormatId& out_format){
    initLoader();
    FIBITMAP *bitmap = new FIBITMAP();
    void* outData = data;
    out_format = NOT_SPECIALIZED;
    if (readInImage(filename, &bitmap)){
        unsigned int width = FreeImage_GetWidth(bitmap);
        unsigned int height = FreeImage_GetHeight(bitmap);
        out_dim = uvec2(width, height);
        out_format = getDataFormatFromBitmap(bitmap);
        switch (out_format)
        {
        case NOT_SPECIALIZED:
            LogErrorCustom("loadImageToData", "Invalid format");
            break;
#define DataFormatIdMacro(i) case inviwo::i: outData = fiBitmapToDataArray<Data##i::type>(data, bitmap, Data##i::bitsAllocated(), Data##i::components()); break;
#include <inviwo/core/util/formatsdefinefunc.h>
        default:
            LogErrorCustom("loadImageToData", "Invalid format or not implemented");
            break;
        }
    }
    //FreeImage_Unload(bitmap);
    return outData;
}

void* ImageIO::loadImageToDataAndRescale(void* data, std::string filename, uvec2 dst_dim, DataFormatId& out_format){
    initLoader();
    FIBITMAP* bitmap = new FIBITMAP();
    void* outData = data;
    out_format = NOT_SPECIALIZED;
    if (readInImage(filename, &bitmap)){
        out_format = getDataFormatFromBitmap(bitmap);
        switch (out_format)
        {
        case NOT_SPECIALIZED:
            LogErrorCustom("loadImageToDataAndRescale", "Invalid format");
            break;
#define DataFormatIdMacro(i) case inviwo::i: outData = fiBitmapToDataArrayAndRescale<Data##i::type>(data, bitmap, dst_dim, Data##i::bitsAllocated(), Data##i::components()); break;
#include <inviwo/core/util/formatsdefinefunc.h>
        default:
            LogErrorCustom("loadImageToDataAndRescale", "Invalid format or not implemented");
            break;
        }
    }
    //FreeImage_Unload(bitmap);
    return outData;
}

void* ImageIO::rescaleLayer(const Layer* inputLayer, uvec2 dst_dim) {
    const LayerRAM* layerRam = inputLayer->getRepresentation<LayerRAM>();
    return rescaleLayerRAM(layerRam, dst_dim);
}

void* ImageIO::rescaleLayerRAM(const LayerRAM* srcLayerRam, uvec2 dst_dim) {
    ivwAssert(srcLayerRam!=NULL, "LayerRAM representation does not exist.");

    initLoader();  

    void* rawData = NULL;
    FIBITMAP* bitmap = NULL;
    FREE_IMAGE_TYPE formatType = getFreeImageFormatFromDataFormat(srcLayerRam->getDataFormatId());
    switch (srcLayerRam->getDataFormatId())
    {
    case NOT_SPECIALIZED:
        LogErrorCustom("rescaleLayerRAM", "Invalid format");
        rawData = NULL;
        break;
#define DataFormatIdMacro(i) case inviwo::i: bitmap = handleBitmapCreations<Data##i::type>(static_cast<const Data##i::type*>(srcLayerRam->getData()), formatType, srcLayerRam->getDimension(), Data##i::bitsAllocated(), Data##i::components(), srcLayerRam->getDataFormat()); \
    rawData = fiBitmapToDataArrayAndRescale<Data##i::type>(NULL, bitmap, dst_dim, Data##i::bitsAllocated(), Data##i::components()); break;
#include <inviwo/core/util/formatsdefinefunc.h>
    default:
        LogErrorCustom("rescaleLayerRAM", "Invalid format or not implemented");
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

FIBITMAP* ImageIO::allocateBitmap(FREE_IMAGE_TYPE type, uvec2 dim, size_t bitsPerPixel, int channels){
    unsigned int rMask = FI_RGBA_RED_MASK;
    unsigned int gMask = FI_RGBA_GREEN_MASK;
    unsigned int bMask = FI_RGBA_BLUE_MASK;

    if(channels == 2){
        bMask = 0;
    }
    else if(channels == 1){
        gMask = 0;
        bMask = 0;
    }
    else if(channels == 0){
        rMask = 0;
        gMask = 0;
        bMask = 0;
    }

    return FreeImage_AllocateT(type, static_cast<int>(dim.x), static_cast<int>(dim.y), static_cast<int>(bitsPerPixel), rMask, gMask, bMask);
}

template<typename T>
FIBITMAP* ImageIO::createBitmapFromData(const T* data, FREE_IMAGE_TYPE type, uvec2 dim, size_t bitsPerPixel, int channels, const DataFormatBase* format){
    FIBITMAP* dib = allocateBitmap(type, dim, bitsPerPixel, channels);
    unsigned int bytespp = FreeImage_GetLine(dib) / FreeImage_GetWidth(dib);
    T* bits = (T*)FreeImage_GetBits(dib);

    //Scale normalized float value to from 0 - 1 to 0  - 255
    if(type == FIT_FLOAT || type == FIT_RGBF || type == FIT_RGBAF){
        T value;
        format->floatToValue(255.f, &value);
        for(unsigned int i = 0; i < dim.x * dim.y; i++){
            bits[i] = data[i]*value;
        }
        FIBITMAP* dibConvert = FreeImage_ConvertToStandardType(dib);
        return dibConvert;
    }

    memcpy(bits, data, dim.x*dim.y*bytespp);
    return dib;
}

template<typename T>
FIBITMAP* ImageIO::handleBitmapCreations(const T* data, FREE_IMAGE_TYPE type, uvec2 dim, size_t bitsPerPixel, int channels, const DataFormatBase* format){
    FIBITMAP *bitmap = createBitmapFromData<T>(data, type, dim, bitsPerPixel, channels, format);
    switchChannels(bitmap, dim, channels);
    return bitmap;
}

FIBITMAP* ImageIO::createBitmapFromData(const LayerRAM* inputLayer){
    FREE_IMAGE_TYPE formatType = getFreeImageFormatFromDataFormat(inputLayer->getDataFormatId());
    switch (inputLayer->getDataFormatId())
    {
    case NOT_SPECIALIZED:
        LogErrorCustom("createBitmapFromData", "Invalid format");
        return NULL;
#define DataFormatIdMacro(i) case inviwo::i: return handleBitmapCreations<Data##i::type>(static_cast<const Data##i::type*>(inputLayer->getData()), formatType, inputLayer->getDimension(), Data##i::bitsAllocated(), Data##i::components(), inputLayer->getDataFormat());
#include <inviwo/core/util/formatsdefinefunc.h>
    default:
        LogErrorCustom("createBitmapFromData", "Invalid format or not implemented");
        break;
    }
    return NULL;
}

template<typename T>
void* ImageIO::fiBitmapToDataArray(void* dst, FIBITMAP* bitmap, size_t bitsPerPixel, int channels){
    unsigned int width = FreeImage_GetWidth(bitmap);
    unsigned int height = FreeImage_GetHeight(bitmap);
    FREE_IMAGE_TYPE type = FreeImage_GetImageType(bitmap);
    uvec2 dim(width, height);
    FIBITMAP* bitmapNEW = allocateBitmap(type, dim, bitsPerPixel, channels);
    FreeImage_Paste(bitmapNEW, bitmap, 0, 0, 255);

    switchChannels(bitmapNEW, dim, channels);

    void* pixelValues = static_cast<void*>(FreeImage_GetBits(bitmapNEW));

    if(!dst){
        T* dstAlloc = new T[dim.x*dim.y];
        dst = static_cast<void*>(dstAlloc);
    }

    memcpy(dst, pixelValues, dim.x*dim.y*sizeof(T));
    //FreeImage_Unload(bitmapNEW);
    return dst;
}

template<typename T>
void* ImageIO::fiBitmapToDataArrayAndRescale(void* dst, FIBITMAP* bitmap, uvec2 dst_dim, size_t bitsPerPixel, int channels){
    int width = FreeImage_GetWidth(bitmap);
    int height = FreeImage_GetHeight(bitmap);
    uvec2 dim(width, height);

    if (dim==dst_dim)
        return fiBitmapToDataArray<T>(dst, bitmap, bitsPerPixel, channels);

    FREE_IMAGE_TYPE type = FreeImage_GetImageType(bitmap);
    FIBITMAP *bitmap2 = allocateBitmap(type, dim, bitsPerPixel, channels);
    FreeImage_Paste(bitmap2, bitmap, 0, 0, 255);

    FIBITMAP* bitmapNEW = FreeImage_Rescale(bitmap2, static_cast<int>(dst_dim.x), static_cast<int>(dst_dim.y), FILTER_BILINEAR);
    FreeImage_Unload(bitmap2);

    switchChannels(bitmapNEW, dst_dim, channels);

    void* pixelValues = static_cast<void*>(FreeImage_GetBits(bitmapNEW));

    if(!dst){
        T* dstAlloc = new T[dst_dim.x*dst_dim.y];
        dst = static_cast<void*>(dstAlloc);
    }

    memcpy(dst, pixelValues, dst_dim.x*dst_dim.y*sizeof(T));
    //FreeImage_Unload(bitmapNEW);
    return dst;
}

void ImageIO::initLoader(){
    if (!loader_initialized){
		loader_initialized = true;
		FreeImage_Initialise(1);
	}
}