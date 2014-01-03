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

#ifndef IVW_IMAGELOADER_H
#define IVW_IMAGELOADER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/image/image.h>
#include <inviwo/core/datastructures/image/imageram.h>
#include <ext/freeimage/FreeImage.h>

using namespace inviwo;

class IVW_CORE_API ImageIO{
public:
	ImageIO(){};
	~ImageIO(){
		FreeImage_DeInitialise();
	}
    /**
    * Loads an image through bitmap.
    * @param void* will point to the raw data
    * @param filename is the file that is to be loaded
    * @return DataFormatId the data format
    */
     static DataFormatId loadImageToData(void* data, std::string filename);
    
     /** 
      * \brief Loads an image through bitmap and rescale
      *
      * @param void* will point to the raw data
      * @param std::string filename is the file that is to be loaded
      * @param int dst_width destination width
      * @param int dst_height destination height
      * @return DataFormatId the data format
      */
     static DataFormatId loadImageToDataAndRescale(void* data, std::string filename, int dst_width, int dst_height);
    
    /**
    * Saves an image to a specified filename.
    * @param filename the path including name to file that is to be stored.
    * @param inputImage specifies the image that is to be saved.
    **/
    static void saveImage(const char* filename, const Image* inputImage);

    /** 
     * \brief Rescales ImageRAM representation of given image data
     * 
     * @param Image * inputImage image data that needs to be rescaled
     * @param int dst_width destination width or new width
     * @param int dst_height destination height or new height     
     * @param void* rescaled raw data
     */
    static void* rescaleImage(Image* inputImage, int dst_width, int dst_height);

    /** 
     * \brief Rescales ImageRAM representation uses FILTER_BILINEAR by default.
     * 
     * @param ImageRAM * imageRam representation that needs rescaling
     * @param int dst_width destination width or new width
     * @param int dst_height destination height or new height
     * @param void* rescaled raw data
     */
    static void* rescaleImageRAM(ImageRAM* imageRam, int dst_width, int dst_height);

    static bool isValidImageFile(std::string fileExtension);

private:
    /**
    * Internal function to load a image 
    * @param filename the name of the file to be loaded
    * @param bitmap the bitmap to store the loaded image.
    * @return if the load was successful.
    */
	static bool readInImage(std::string filename, FIBITMAP** bitmap);

    /**
    * Initializes freeimage if needed.
    **/
    static void initLoader();

    /**
    * Create bitmap from image.
    */
    static FIBITMAP* createBitmapFromData(const ImageRAM* inputImage);

    /**
    * Converts image to byte array.
    */
    static FIBITMAP* convertToByte(const ImageRAM* inputImage, uvec2 dim, size_t bitsPerPixel);

    static FIBITMAP* allocateBitmap(int width, int height, size_t bitsPerPixel, int channels);

    template<typename T>
    static FIBITMAP* handleBitmapCreations(const T* data, uvec2 dim, size_t bitsPerPixel, int channels);

    template<typename T>
    static FIBITMAP* createBitmapFromData(const T* data, uvec2 dim, size_t bitsPerPixel, int channels);

    /**
    * Switch red and blue channels in the bitmap.
    */
    static void switchChannels(FIBITMAP* bitmap, uvec2 dim, int channels);

    /**
    * Converts an image from freeimage format to regular int.
    **/
    template<typename T>
    static void fiBitmapToDataArray(void* dst, FIBITMAP* bitmap, size_t bitsPerPixel, int channels);

    /** 
     * \brief fits the bitmap into data array which is readable by representations such as ImageRAM that uses FILTER_BILINEAR
     */
    template<typename T>
    static void fiBitmapToDataArrayAndRescale(void* dst, FIBITMAP* bitmap, int dst_width, int dst_height, size_t bitsPerPixel, int channels);

	static bool loader_initialized;
};
#endif // IVW_IMAGELOADER_H