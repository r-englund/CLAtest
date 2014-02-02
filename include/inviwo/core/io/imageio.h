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
#include <inviwo/core/datastructures/image/layer.h>
#include <inviwo/core/datastructures/image/layerram.h>
#include <ext/freeimage/FreeImage.h>

using namespace inviwo;

class IVW_CORE_API ImageIO {
public:
    ImageIO() {};
    ~ImageIO() {
        FreeImage_DeInitialise();
    }
    /**
    * Loads an image through bitmap.
    * @param void* will point to the raw data
    * @param filename is the file that is to be loaded
    * @return DataFormatId the data format
    */
    static void* loadImageToData(void* data, std::string filename, uvec2& out_dim, DataFormatId& out_format);

    /**
     * \brief Loads an image through bitmap and rescale
     *
     * @param void* will point to the raw data
     * @param std::string filename is the file that is to be loaded
     * @param uvec2 dst_dim is destination dimensions
     * @return DataFormatId the data format
     */
    static void* loadImageToDataAndRescale(void* data, std::string filename, uvec2 dist_dim, DataFormatId& out_format);

    /**
    * Saves an layer of an image to a specified filename.
    * @param filename the path including name to file that is to be stored.
    * @param inputImage specifies the image that is to be saved.
    **/
    static void saveLayer(const char* filename, const Layer* inputImage);

    /**
     * \brief Rescales Layer of given image data
     *
     * @param Layer * inputImage image data that needs to be rescaled
     * @param uvec2 dst_dim is destination dimensions
     * @param void* rescaled raw data
     */
    static void* rescaleLayer(const Layer* inputLayer, uvec2 dst_dim);

    /**
     * \brief Rescales LayerRAM representation uses FILTER_BILINEAR by default.
     *
     * @param LayerRAM * imageRam representation that needs rescaling
     * @param uvec2 dst_dim is destination dimensions
     * @param void* rescaled raw data
     */
    static void* rescaleLayerRAM(const LayerRAM* layerRam, uvec2 dst_dim);

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
    static FIBITMAP* createBitmapFromData(const LayerRAM* inputImage);

    /**
    * Converts image to byte array.
    */
    static FIBITMAP* convertToByte(const LayerRAM* inputImage, uvec2 dim, size_t bitsPerPixel);

    static FIBITMAP* allocateBitmap(FREE_IMAGE_TYPE type, uvec2 dim, size_t bitsPerPixel, int channels);

    template<typename T>
    static FIBITMAP* handleBitmapCreations(const T* data, FREE_IMAGE_TYPE type, uvec2 dim, size_t bitsPerPixel, int channels,
                                           const DataFormatBase* format);

    template<typename T>
    static FIBITMAP* createBitmapFromData(const T* data, FREE_IMAGE_TYPE type, uvec2 dim, size_t bitsPerPixel, int channels,
                                          const DataFormatBase* format);

    /**
    * Switch red and blue channels in the bitmap.
    */
    static void switchChannels(FIBITMAP* bitmap, uvec2 dim, int channels);

    /**
    * Converts an image from freeimage format to regular int.
    **/
    template<typename T>
    static void* fiBitmapToDataArray(void* dst, FIBITMAP* bitmap, size_t bitsPerPixel, int channels);

    /**
     * \brief fits the bitmap into data array which is readable by representations such as LayerRAM that uses FILTER_BILINEAR
     */
    template<typename T>
    static void* fiBitmapToDataArrayAndRescale(void* dst, FIBITMAP* bitmap, uvec2 dst_dim, size_t bitsPerPixel, int channels);

    static bool loader_initialized;
};
#endif // IVW_IMAGELOADER_H