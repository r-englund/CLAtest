/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Contact: Peter Steneteg
 *
 *********************************************************************************/

#ifndef IVW_SPATIALDATA_H
#define IVW_SPATIALDATA_H

#pragma warning(disable : 4355)

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {

/** \brief A convenience class to generate transformation matrices between
 *         the different coordinate systems in use.
 *
 *  Spatial meta data in Inviwo uses 4 different coordinate systems, they are defined as
 *  - Index - The voxel indices in the data
 *  - Data  - The corresponding texture coordinates of the data.
 *  - Model - Defines a local basis and offset for the data.
 *  - World - Puts the data at a position and angle in the scene.
 *
 *  A matrix is always stored in a 1 dim array, for example a 4x4 matrix would be:
 *  m = (1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16)
 *  in c/c++, that uses row major, that translates to a matrix like
 *
 *      m[0][0]=1  m[0][1]=2  m[0][2]=3  m[0][3]=4
 *      m[1][0]=5  m[1][1]=6  m[1][2]=7  m[1][3]=8
 *      m[2][0]=9  m[2][1]=10 m[2][2]=11 m[2][3]=12
 *      m[3][0]=13 m[3][1]=14 m[3][2]=15 m[3][3]=16
 *
 *  here the first index represent the row and the second the column: m[row][column].
 *  On the gpu, that uses column major, the same array would look like:
 *
 *      m[0][0]=1  m[1][0]=5  m[2][0]=9  m[3][0]=13
 *      m[0][1]=2  m[1][1]=6  m[2][1]=10 m[3][1]=14
 *      m[0][2]=3  m[1][2]=7  m[2][2]=11 m[3][2]=15
 *      m[0][3]=4  m[1][3]=8  m[2][3]=12 m[3][3]=16
 *
 *  here the first index is the column and the second the row: m[column][row]
 *
 *  For example to create a translation matrix for on the gpu you want:
 *
 *      1  0  0 dx
 *      0  1  0 dy
 *      0  0  0 dz
 *      0  0  0  1
 *
 *  That means that in c/c++ you would create a transposed matrix like:
 *
 *      1  0  0  0
 *      0  1  0  0
 *      0  0  1  0
 *      dx dy dz 1
 *
 *  GLM also uses column major hence in glm you write m[column][row]
 *  hence you would enter the a translation like:
 *
 *      m[0][0]=1  m[1][0]=0  m[2][0]=0  m[3][0]=dx
 *      m[0][1]=0  m[1][1]=1  m[2][1]=0  m[3][1]=dy
 *      m[0][2]=0  m[1][2]=0  m[2][2]=1  m[3][2]=dz
 *      m[0][3]=0  m[1][3]=0  m[2][3]=0  m[3][3]=1
 *
 *  This means that they have the same representation as on the gpu.
 */

template <unsigned int N>
class SpatialCoordinateTransformer {
public:
    virtual ~SpatialCoordinateTransformer() {}
    virtual SpatialCoordinateTransformer<N>* clone() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to model space coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to (data
     * min, data max)
     */
    virtual const Matrix<N + 1, float> getDataToModelMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to world space coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to (-inf,
     * inf)
     */
    virtual const Matrix<N + 1, float> getDataToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to raw data numbers, i.e. from (data min, data max) to generally (-inf, inf), ([0,1] for
     * textures)
     */
    virtual const Matrix<N + 1, float> getModelToDataMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to world space coordinates, i.e. from (data min, data max) to (-inf, inf)
     */
    virtual const Matrix<N + 1, float> getModelToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to raw data numbers, i.e. from (-inf, inf) to generally (-inf, inf), ([0,1] for textures)
     */
    virtual const Matrix<N + 1, float> getWorldToDataMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to model space coordinates, i.e. from (-inf, inf) to (data min, data max)
     */
    virtual const Matrix<N + 1, float> getWorldToModelMatrix() const = 0;
};

template <unsigned int N>
class StructuredCoordinateTransformer : public SpatialCoordinateTransformer<N> {
public:
    virtual ~StructuredCoordinateTransformer() {}
    virtual StructuredCoordinateTransformer<N>* clone() const = 0;
    /**
     * Returns the matrix transformation mapping from voxel index coordinates
     * to raw data numbers, i.e. from [0, number of voxels) to generally (-inf, inf), ([0,1] for
     * textures)
     */
    virtual const Matrix<N + 1, float> getIndexToDataMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from voxel index coordinates
     * to model space coordinates, i.e. from [0, number of voxels) to (data min, data max)
     */
    virtual const Matrix<N + 1, float> getIndexToModelMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from voxel index coordinates
     * to world space coordinates, i.e. from [0, number of voxels) to (-inf, inf)
     */
    virtual const Matrix<N + 1, float> getIndexToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to voxel index coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to [0,
     * number of voxels)
     */
    virtual const Matrix<N + 1, float> getDataToIndexMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to model space coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to (data
     * min, data max)
     */
    virtual const Matrix<N + 1, float> getDataToModelMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to world space coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to (-inf,
     * inf)
     */
    virtual const Matrix<N + 1, float> getDataToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to voxel index coordinates, i.e. from (data min, data max) to [0, number of voxels)
     */
    virtual const Matrix<N + 1, float> getModelToIndexMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to raw data numbers, i.e. from (data min, data max) to generally (-inf, inf), ([0,1] for
     * textures)
     */
    virtual const Matrix<N + 1, float> getModelToDataMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to world space coordinates, i.e. from (data min, data max) to (-inf, inf)
     */
    virtual const Matrix<N + 1, float> getModelToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to voxel index coordinates, i.e. from (-inf, inf) to [0, number of voxels)
     */
    virtual const Matrix<N + 1, float> getWorldToIndexMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to raw data numbers, i.e. from (-inf, inf) to generally (-inf, inf), ([0,1] for textures)
     */
    virtual const Matrix<N + 1, float> getWorldToDataMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to model space coordinates, i.e. from (-inf, inf) to (data min, data max)
     */
    virtual const Matrix<N + 1, float> getWorldToModelMatrix() const = 0;
};

template <unsigned int N>
class SpatialCameraCoordinateTransformer : public SpatialCoordinateTransformer<N> {
public:
    virtual ~SpatialCameraCoordinateTransformer() {}
    virtual SpatialCameraCoordinateTransformer<N>* clone() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to model space coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to (data
     * min, data max)
     */
    virtual const Matrix<N + 1, float> getDataToModelMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to world space coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to (-inf,
     * inf)
     */
    virtual const Matrix<N + 1, float> getDataToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to view space coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to (-inf,
     * inf)
     */
    virtual const Matrix<N + 1, float> getDataToViewMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to clip space coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to [-1,1]
     */
    virtual const Matrix<N + 1, float> getDataToClipMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to raw data numbers, i.e. from (data min, data max) to generally (-inf, inf), ([0,1] for
     * textures)
     */
    virtual const Matrix<N + 1, float> getModelToDataMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to world space coordinates, i.e. from (data min, data max) to (-inf, inf)
     */
    virtual const Matrix<N + 1, float> getModelToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to view space coordinates, i.e. from (data min, data max) to (-inf, inf)
     */
    virtual const Matrix<N + 1, float> getModelToViewMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to clip space coordinates, i.e. from (data min, data max) to [-1,1]
     */
    virtual const Matrix<N + 1, float> getModelToClipMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to raw data numbers, i.e. from (-inf, inf) to generally (-inf, inf), ([0,1] for textures)
     */
    virtual const Matrix<N + 1, float> getWorldToDataMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to model space coordinates, i.e. from (-inf, inf) to (data min, data max)
     */
    virtual const Matrix<N + 1, float> getWorldToModelMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to view space coordinates, i.e. from (-inf, inf) to (-inf, inf)
     */
    virtual const Matrix<N + 1, float> getWorldToViewMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to clip space coordinates, i.e. from (-inf, inf) to [-1,1]
     */
    virtual const Matrix<N + 1, float> getWorldToClipMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from view space coordinates
     * to raw data numbers, i.e. from (-inf, inf) to generally (-inf, inf), ([0,1] for textures)
     */
    virtual const Matrix<N + 1, float> getViewToDataMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from view space coordinates
     * to model space coordinates, i.e. from (-inf, inf) to (data min, data max)
     */
    virtual const Matrix<N + 1, float> getViewToModelMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from view space coordinates
     * to world space coordinates, i.e. from (-inf, inf) to (-inf, inf)
     */
    virtual const Matrix<N + 1, float> getViewToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from view space coordinates
     * to clip space coordinates, i.e. from (-inf, inf) to [-1,1]
     */
    virtual const Matrix<N + 1, float> getViewToClipMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from clip space coordinates
     * to raw data numbers, i.e. from [-1,1] to generally (-inf, inf), ([0,1] for textures)
     */
    virtual const Matrix<N + 1, float> getClipToDataMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from clip space coordinates
     * to model space coordinates, i.e. from [-1,1] to (data min, data max)
     */
    virtual const Matrix<N + 1, float> getClipToModelMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from clip space coordinates
     * to world space coordinates, i.e. from [-1,1] to (-inf, inf)
     */
    virtual const Matrix<N + 1, float> getClipToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from clip space coordinates
     * to view space coordinates, i.e. from [-1,1] to (-inf, inf)
     */
    virtual const Matrix<N + 1, float> getClipToViewMatrix() const = 0;
};

template <unsigned int N>
class StructuedCameraCoordinateTransformer : public SpatialCameraCoordinateTransformer<N> {
public:
    virtual ~StructuedCameraCoordinateTransformer() {}
    virtual StructuedCameraCoordinateTransformer<N>* clone() const = 0;
    /**
     * Returns the matrix transformation mapping from voxel index coordinates
     * to raw data numbers, i.e. from [0, number of voxels) to generally (-inf, inf), ([0,1] for
     * textures)
     */
    virtual const Matrix<N + 1, float> getIndexToDataMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from voxel index coordinates
     * to model space coordinates, i.e. from [0, number of voxels) to (data min, data max)
     */
    virtual const Matrix<N + 1, float> getIndexToModelMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from voxel index coordinates
     * to world space coordinates, i.e. from [0, number of voxels) to (-inf, inf)
     */
    virtual const Matrix<N + 1, float> getIndexToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from voxel index coordinates
     * to view space coordinates, i.e. from [0, number of voxels) to (-inf, inf)
     */
    virtual const Matrix<N + 1, float> getIndexToViewMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from voxel index coordinates
     * to clip space coordinates, i.e. from [0, number of voxels) to [-1,1]
     */
    virtual const Matrix<N + 1, float> getIndexToClipMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to voxel index coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to [0,
     * number of voxels)
     */
    virtual const Matrix<N + 1, float> getDataToIndexMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to model space coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to (data
     * min, data max)
     */
    virtual const Matrix<N + 1, float> getDataToModelMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to world space coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to (-inf,
     * inf)
     */
    virtual const Matrix<N + 1, float> getDataToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to view space coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to (-inf,
     * inf)
     */
    virtual const Matrix<N + 1, float> getDataToViewMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to clip space coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to [-1,1]
     */
    virtual const Matrix<N + 1, float> getDataToClipMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to voxel index coordinates, i.e. from (data min, data max) to [0, number of voxels)
     */
    virtual const Matrix<N + 1, float> getModelToIndexMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to raw data numbers, i.e. from (data min, data max) to generally (-inf, inf), ([0,1] for
     * textures)
     */
    virtual const Matrix<N + 1, float> getModelToDataMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to world space coordinates, i.e. from (data min, data max) to (-inf, inf)
     */
    virtual const Matrix<N + 1, float> getModelToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to view space coordinates, i.e. from (data min, data max) to (-inf, inf)
     */
    virtual const Matrix<N + 1, float> getModelToViewMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to clip space coordinates, i.e. from (data min, data max) to [-1,1]
     */
    virtual const Matrix<N + 1, float> getModelToClipMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to voxel index coordinates, i.e. from (-inf, inf) to [0, number of voxels)
     */
    virtual const Matrix<N + 1, float> getWorldToIndexMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to raw data numbers, i.e. from (-inf, inf) to generally (-inf, inf), ([0,1] for textures)
     */
    virtual const Matrix<N + 1, float> getWorldToDataMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to model space coordinates, i.e. from (-inf, inf) to (data min, data max)
     */
    virtual const Matrix<N + 1, float> getWorldToModelMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to view space coordinates, i.e. from (-inf, inf) to (-inf, inf)
     */
    virtual const Matrix<N + 1, float> getWorldToViewMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to clip space coordinates, i.e. from (-inf, inf) to [-1,1]
     */
    virtual const Matrix<N + 1, float> getWorldToClipMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from view space coordinates
     * to voxel index coordinates, i.e. from (-inf, inf) to [0, number of voxels)
     */
    virtual const Matrix<N + 1, float> getViewToIndexMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from view space coordinates
     * to raw data numbers, i.e. from (-inf, inf) to generally (-inf, inf), ([0,1] for textures)
     */
    virtual const Matrix<N + 1, float> getViewToDataMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from view space coordinates
     * to model space coordinates, i.e. from (-inf, inf) to (data min, data max)
     */
    virtual const Matrix<N + 1, float> getViewToModelMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from view space coordinates
     * to world space coordinates, i.e. from (-inf, inf) to (-inf, inf)
     */
    virtual const Matrix<N + 1, float> getViewToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from view space coordinates
     * to clip space coordinates, i.e. from (-inf, inf) to [-1,1]
     */
    virtual const Matrix<N + 1, float> getViewToClipMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from clip space coordinates
     * to voxel index coordinates, i.e. from [-1,1] to [0, number of voxels)
     */
    virtual const Matrix<N + 1, float> getClipToIndexMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from clip space coordinates
     * to raw data numbers, i.e. from [-1,1] to generally (-inf, inf), ([0,1] for textures)
     */
    virtual const Matrix<N + 1, float> getClipToDataMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from clip space coordinates
     * to model space coordinates, i.e. from [-1,1] to (data min, data max)
     */
    virtual const Matrix<N + 1, float> getClipToModelMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from clip space coordinates
     * to world space coordinates, i.e. from [-1,1] to (-inf, inf)
     */
    virtual const Matrix<N + 1, float> getClipToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from clip space coordinates
     * to view space coordinates, i.e. from [-1,1] to (-inf, inf)
     */
    virtual const Matrix<N + 1, float> getClipToViewMatrix() const = 0;
};

template <unsigned int N>
class VolumeCoordinateTransformer : public SpatialCoordinateTransformer<N> {
public:
    virtual ~VolumeCoordinateTransformer() {}
    virtual VolumeCoordinateTransformer<N>* clone() const = 0;
    /**
     * Returns the matrix transformation mapping from voxel index coordinates
     * to raw data numbers, i.e. from [0, number of voxels) to generally (-inf, inf), ([0,1] for
     * textures)
     */
    virtual const Matrix<N + 1, float> getIndexToTextureMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from voxel index coordinates
     * to model space coordinates, i.e. from [0, number of voxels) to (data min, data max)
     */
    virtual const Matrix<N + 1, float> getIndexToModelMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from voxel index coordinates
     * to world space coordinates, i.e. from [0, number of voxels) to (-inf, inf)
     */
    virtual const Matrix<N + 1, float> getIndexToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to voxel index coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to [0,
     * number of voxels)
     */
    virtual const Matrix<N + 1, float> getTextureToIndexMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to model space coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to (data
     * min, data max)
     */
    virtual const Matrix<N + 1, float> getTextureToModelMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to world space coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to (-inf,
     * inf)
     */
    virtual const Matrix<N + 1, float> getTextureToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to voxel index coordinates, i.e. from (data min, data max) to [0, number of voxels)
     */
    virtual const Matrix<N + 1, float> getModelToIndexMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to raw data numbers, i.e. from (data min, data max) to generally (-inf, inf), ([0,1] for
     * textures)
     */
    virtual const Matrix<N + 1, float> getModelToTextureMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to world space coordinates, i.e. from (data min, data max) to (-inf, inf)
     */
    virtual const Matrix<N + 1, float> getModelToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to voxel index coordinates, i.e. from (-inf, inf) to [0, number of voxels)
     */
    virtual const Matrix<N + 1, float> getWorldToIndexMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to raw data numbers, i.e. from (-inf, inf) to generally (-inf, inf), ([0,1] for textures)
     */
    virtual const Matrix<N + 1, float> getWorldToTextureMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to model space coordinates, i.e. from (-inf, inf) to (data min, data max)
     */
    virtual const Matrix<N + 1, float> getWorldToModelMatrix() const = 0;
};

template <unsigned int N>
class VolumeCameraCoordinateTransformer : public SpatialCameraCoordinateTransformer<N> {
public:
    virtual ~VolumeCameraCoordinateTransformer() {}
    virtual VolumeCameraCoordinateTransformer<N>* clone() const = 0;
    /**
     * Returns the matrix transformation mapping from voxel index coordinates
     * to raw data numbers, i.e. from [0, number of voxels) to generally (-inf, inf), ([0,1] for
     * textures)
     */
    virtual const Matrix<N + 1, float> getIndexToTextureMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from voxel index coordinates
     * to model space coordinates, i.e. from [0, number of voxels) to (data min, data max)
     */
    virtual const Matrix<N + 1, float> getIndexToModelMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from voxel index coordinates
     * to world space coordinates, i.e. from [0, number of voxels) to (-inf, inf)
     */
    virtual const Matrix<N + 1, float> getIndexToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from voxel index coordinates
     * to view space coordinates, i.e. from [0, number of voxels) to (-inf, inf)
     */
    virtual const Matrix<N + 1, float> getIndexToViewMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from voxel index coordinates
     * to clip space coordinates, i.e. from [0, number of voxels) to [-1,1]
     */
    virtual const Matrix<N + 1, float> getIndexToClipMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to voxel index coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to [0,
     * number of voxels)
     */
    virtual const Matrix<N + 1, float> getTextureToIndexMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to model space coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to (data
     * min, data max)
     */
    virtual const Matrix<N + 1, float> getTextureToModelMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to world space coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to (-inf,
     * inf)
     */
    virtual const Matrix<N + 1, float> getTextureToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to view space coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to (-inf,
     * inf)
     */
    virtual const Matrix<N + 1, float> getTextureToViewMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to clip space coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to [-1,1]
     */
    virtual const Matrix<N + 1, float> getTextureToClipMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to voxel index coordinates, i.e. from (data min, data max) to [0, number of voxels)
     */
    virtual const Matrix<N + 1, float> getModelToIndexMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to raw data numbers, i.e. from (data min, data max) to generally (-inf, inf), ([0,1] for
     * textures)
     */
    virtual const Matrix<N + 1, float> getModelToTextureMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to world space coordinates, i.e. from (data min, data max) to (-inf, inf)
     */
    virtual const Matrix<N + 1, float> getModelToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to view space coordinates, i.e. from (data min, data max) to (-inf, inf)
     */
    virtual const Matrix<N + 1, float> getModelToViewMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to clip space coordinates, i.e. from (data min, data max) to [-1,1]
     */
    virtual const Matrix<N + 1, float> getModelToClipMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to voxel index coordinates, i.e. from (-inf, inf) to [0, number of voxels)
     */
    virtual const Matrix<N + 1, float> getWorldToIndexMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to raw data numbers, i.e. from (-inf, inf) to generally (-inf, inf), ([0,1] for textures)
     */
    virtual const Matrix<N + 1, float> getWorldToTextureMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to model space coordinates, i.e. from (-inf, inf) to (data min, data max)
     */
    virtual const Matrix<N + 1, float> getWorldToModelMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to view space coordinates, i.e. from (-inf, inf) to (-inf, inf)
     */
    virtual const Matrix<N + 1, float> getWorldToViewMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to clip space coordinates, i.e. from (-inf, inf) to [-1,1]
     */
    virtual const Matrix<N + 1, float> getWorldToClipMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from view space coordinates
     * to voxel index coordinates, i.e. from (-inf, inf) to [0, number of voxels)
     */
    virtual const Matrix<N + 1, float> getViewToIndexMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from view space coordinates
     * to raw data numbers, i.e. from (-inf, inf) to generally (-inf, inf), ([0,1] for textures)
     */
    virtual const Matrix<N + 1, float> getViewToTextureMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from view space coordinates
     * to model space coordinates, i.e. from (-inf, inf) to (data min, data max)
     */
    virtual const Matrix<N + 1, float> getViewToModelMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from view space coordinates
     * to world space coordinates, i.e. from (-inf, inf) to (-inf, inf)
     */
    virtual const Matrix<N + 1, float> getViewToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from view space coordinates
     * to clip space coordinates, i.e. from (-inf, inf) to [-1,1]
     */
    virtual const Matrix<N + 1, float> getViewToClipMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from clip space coordinates
     * to voxel index coordinates, i.e. from [-1,1] to [0, number of voxels)
     */
    virtual const Matrix<N + 1, float> getClipToIndexMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from clip space coordinates
     * to raw data numbers, i.e. from [-1,1] to generally (-inf, inf), ([0,1] for textures)
     */
    virtual const Matrix<N + 1, float> getClipToTextureMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from clip space coordinates
     * to model space coordinates, i.e. from [-1,1] to (data min, data max)
     */
    virtual const Matrix<N + 1, float> getClipToModelMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from clip space coordinates
     * to world space coordinates, i.e. from [-1,1] to (-inf, inf)
     */
    virtual const Matrix<N + 1, float> getClipToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from clip space coordinates
     * to view space coordinates, i.e. from [-1,1] to (-inf, inf)
     */
    virtual const Matrix<N + 1, float> getClipToViewMatrix() const = 0;
};

template <unsigned int N>
class Camera;

template <unsigned int N>
class SpatialEntity {
public:
    SpatialEntity();
    SpatialEntity(const SpatialEntity<N>& rhs);
    SpatialEntity(const Matrix<N + 1, float>& modelMatrix);
    SpatialEntity(const Matrix<N + 1, float>& modelMatrix, const Matrix<N + 1, float>& worldMatrix);

    SpatialEntity<N>& operator=(const SpatialEntity<N>& that);
    virtual SpatialEntity<N>* clone() const = 0;
    virtual ~SpatialEntity();

    Vector<N, float> getOffset() const;
    void setOffset(const Vector<N, float>& offset);

    // Using row vectors in basis
    Matrix<N, float> getBasis() const;
    void setBasis(const Matrix<N, float>& basis);

    Matrix<N + 1, float> getModelMatrix() const;
    void setModelMatrix(const Matrix<N + 1, float>& modelMatrix);

    Matrix<N + 1, float> getWorldMatrix() const;
    void setWorldMatrix(const Matrix<N + 1, float>& worldMatrix);

    virtual const SpatialCoordinateTransformer<N>& getCoordinateTransformer() const;

protected:
    SpatialEntity(SpatialCoordinateTransformer<N>* transformer);
    SpatialEntity(SpatialCoordinateTransformer<N>* transformer,
                  const Matrix<N + 1, float>& modelMatrix);
    SpatialEntity(SpatialCoordinateTransformer<N>* transformer,
                  const Matrix<N + 1, float>& modelMatrix, const Matrix<N + 1, float>& worldMatrix);

    SpatialCoordinateTransformer<N>* transformer_;
    Matrix<N + 1, float> modelMatrix_;
    Matrix<N + 1, float> worldMatrix_;
};

template <unsigned int N>
class StructuredGridEntity : public SpatialEntity<N> {
public:
    StructuredGridEntity();
    StructuredGridEntity(const StructuredGridEntity<N>& rhs);
    StructuredGridEntity(const Vector<N, unsigned int>& dimension);
    StructuredGridEntity(const Vector<N, unsigned int>& dimension, const Vector<N, float>& spacing);
    StructuredGridEntity(const Vector<N, unsigned int>& dimension,
                         const Matrix<N + 1, float>& modelMatrix);
    StructuredGridEntity(const Vector<N, unsigned int>& dimension,
                         const Matrix<N + 1, float>& modelMatrix,
                         const Matrix<N + 1, float>& worldMatrix);

    StructuredGridEntity<N>& operator=(const StructuredGridEntity<N>& that);
    virtual StructuredGridEntity<N>* clone() const = 0;

    virtual ~StructuredGridEntity() {}

    Vector<N, unsigned int> getDimension() const;
    void setDimension(const Vector<N, unsigned int>& dimension);

    Matrix<N + 1, float> getIndexMatrix() const;

    virtual const StructuredCoordinateTransformer<N>& getCoordinateTransformer() const;

protected:
    Vector<N, unsigned int> dimension_;
};

template <unsigned int N>
class SpatialCoordinateTransformerImpl : public SpatialCoordinateTransformer<N> {
public:
    SpatialCoordinateTransformerImpl(const SpatialEntity<N>* spatialEntity);
    SpatialCoordinateTransformerImpl(const SpatialCoordinateTransformerImpl<N>& rhs);
    SpatialCoordinateTransformerImpl<N>& operator=(const SpatialCoordinateTransformerImpl<N>& that);
    virtual SpatialCoordinateTransformerImpl<N>* clone() const;
    virtual ~SpatialCoordinateTransformerImpl() {}

    virtual const Matrix<N + 1, float> getModelMatrix() const;
    virtual const Matrix<N + 1, float> getWorldMatrix() const;

    virtual const Matrix<N + 1, float> getDataToModelMatrix() const;
    virtual const Matrix<N + 1, float> getDataToWorldMatrix() const;
    virtual const Matrix<N + 1, float> getModelToDataMatrix() const;
    virtual const Matrix<N + 1, float> getModelToWorldMatrix() const;
    virtual const Matrix<N + 1, float> getWorldToDataMatrix() const;
    virtual const Matrix<N + 1, float> getWorldToModelMatrix() const;

private:
    const SpatialEntity<N>* spatialEntity_;
};

template <unsigned int N>
class StructuredCoordinateTransformerImpl : public StructuredCoordinateTransformer<N> {
public:
    StructuredCoordinateTransformerImpl(const StructuredGridEntity<N>* structuredGridEntity);
    StructuredCoordinateTransformerImpl(const StructuredCoordinateTransformerImpl<N>& rhs);
    StructuredCoordinateTransformerImpl<N>& operator=(
        const StructuredCoordinateTransformerImpl<N>& that);
    virtual StructuredCoordinateTransformerImpl<N>* clone() const;
    virtual ~StructuredCoordinateTransformerImpl() {}

    virtual const Matrix<N + 1, float> getIndexMatrix() const;
    virtual const Matrix<N + 1, float> getModelMatrix() const;
    virtual const Matrix<N + 1, float> getWorldMatrix() const;

    virtual const Matrix<N + 1, float> getIndexToDataMatrix() const;
    virtual const Matrix<N + 1, float> getIndexToModelMatrix() const;
    virtual const Matrix<N + 1, float> getIndexToWorldMatrix() const;
    virtual const Matrix<N + 1, float> getDataToIndexMatrix() const;
    virtual const Matrix<N + 1, float> getDataToModelMatrix() const;
    virtual const Matrix<N + 1, float> getDataToWorldMatrix() const;
    virtual const Matrix<N + 1, float> getModelToIndexMatrix() const;
    virtual const Matrix<N + 1, float> getModelToDataMatrix() const;
    virtual const Matrix<N + 1, float> getModelToWorldMatrix() const;
    virtual const Matrix<N + 1, float> getWorldToIndexMatrix() const;
    virtual const Matrix<N + 1, float> getWorldToDataMatrix() const;
    virtual const Matrix<N + 1, float> getWorldToModelMatrix() const;

private:
    const StructuredGridEntity<N>* structuredGridEntity_;
};

template <unsigned int N>
class SpatialCameraCoordinateTransformerImpl : public SpatialCameraCoordinateTransformer<N> {
public:
    SpatialCameraCoordinateTransformerImpl(const SpatialEntity<N>* spatialEntity,
                                           const Camera<N>* camera);
    SpatialCameraCoordinateTransformerImpl(const SpatialCameraCoordinateTransformerImpl<N>& rhs);
    SpatialCameraCoordinateTransformerImpl<N>& operator=(
        const SpatialCameraCoordinateTransformerImpl<N>& that);
    virtual SpatialCameraCoordinateTransformerImpl<N>* clone() const;
    virtual ~SpatialCameraCoordinateTransformerImpl() {}

    virtual const Matrix<N + 1, float> getModelMatrix() const;
    virtual const Matrix<N + 1, float> getWorldMatrix() const;
    virtual const Matrix<N + 1, float> getViewMatrix() const;
    virtual const Matrix<N + 1, float> getProjectionMatrix() const;

    virtual const Matrix<N + 1, float> getDataToModelMatrix() const;
    virtual const Matrix<N + 1, float> getDataToWorldMatrix() const;
    virtual const Matrix<N + 1, float> getDataToViewMatrix() const;
    virtual const Matrix<N + 1, float> getDataToClipMatrix() const;
    virtual const Matrix<N + 1, float> getModelToDataMatrix() const;
    virtual const Matrix<N + 1, float> getModelToWorldMatrix() const;
    virtual const Matrix<N + 1, float> getModelToViewMatrix() const;
    virtual const Matrix<N + 1, float> getModelToClipMatrix() const;
    virtual const Matrix<N + 1, float> getWorldToDataMatrix() const;
    virtual const Matrix<N + 1, float> getWorldToModelMatrix() const;
    virtual const Matrix<N + 1, float> getWorldToViewMatrix() const;
    virtual const Matrix<N + 1, float> getWorldToClipMatrix() const;
    virtual const Matrix<N + 1, float> getViewToDataMatrix() const;
    virtual const Matrix<N + 1, float> getViewToModelMatrix() const;
    virtual const Matrix<N + 1, float> getViewToWorldMatrix() const;
    virtual const Matrix<N + 1, float> getViewToClipMatrix() const;
    virtual const Matrix<N + 1, float> getClipToDataMatrix() const;
    virtual const Matrix<N + 1, float> getClipToModelMatrix() const;
    virtual const Matrix<N + 1, float> getClipToWorldMatrix() const;
    virtual const Matrix<N + 1, float> getClipToViewMatrix() const;

protected:
    const SpatialEntity<N>* spatialEntity_;
    const Camera<N>* camera_;
};

template <unsigned int N>
class StructuedCameraCoordinateTransformerImpl : public StructuedCameraCoordinateTransformer<N> {
public:
    StructuedCameraCoordinateTransformerImpl(const StructuredGridEntity<N>* structuredGridEntity,
                                             const Camera<N>* camera);
    StructuedCameraCoordinateTransformerImpl(
        const StructuedCameraCoordinateTransformerImpl<N>& rhs);
    StructuedCameraCoordinateTransformerImpl<N>& operator=(
        const StructuedCameraCoordinateTransformerImpl<N>& that);
    virtual StructuedCameraCoordinateTransformerImpl<N>* clone() const;
    virtual ~StructuedCameraCoordinateTransformerImpl() {}

    virtual const Matrix<N + 1, float> getIndexMatrix() const;
    virtual const Matrix<N + 1, float> getModelMatrix() const;
    virtual const Matrix<N + 1, float> getWorldMatrix() const;
    virtual const Matrix<N + 1, float> getViewMatrix() const;
    virtual const Matrix<N + 1, float> getProjectionMatrix() const;

    virtual const Matrix<N + 1, float> getIndexToDataMatrix() const;
    virtual const Matrix<N + 1, float> getIndexToModelMatrix() const;
    virtual const Matrix<N + 1, float> getIndexToWorldMatrix() const;
    virtual const Matrix<N + 1, float> getIndexToViewMatrix() const;
    virtual const Matrix<N + 1, float> getIndexToClipMatrix() const;
    virtual const Matrix<N + 1, float> getDataToIndexMatrix() const;
    virtual const Matrix<N + 1, float> getDataToModelMatrix() const;
    virtual const Matrix<N + 1, float> getDataToWorldMatrix() const;
    virtual const Matrix<N + 1, float> getDataToViewMatrix() const;
    virtual const Matrix<N + 1, float> getDataToClipMatrix() const;
    virtual const Matrix<N + 1, float> getModelToIndexMatrix() const;
    virtual const Matrix<N + 1, float> getModelToDataMatrix() const;
    virtual const Matrix<N + 1, float> getModelToWorldMatrix() const;
    virtual const Matrix<N + 1, float> getModelToViewMatrix() const;
    virtual const Matrix<N + 1, float> getModelToClipMatrix() const;
    virtual const Matrix<N + 1, float> getWorldToIndexMatrix() const;
    virtual const Matrix<N + 1, float> getWorldToDataMatrix() const;
    virtual const Matrix<N + 1, float> getWorldToModelMatrix() const;
    virtual const Matrix<N + 1, float> getWorldToViewMatrix() const;
    virtual const Matrix<N + 1, float> getWorldToClipMatrix() const;
    virtual const Matrix<N + 1, float> getViewToIndexMatrix() const;
    virtual const Matrix<N + 1, float> getViewToDataMatrix() const;
    virtual const Matrix<N + 1, float> getViewToModelMatrix() const;
    virtual const Matrix<N + 1, float> getViewToWorldMatrix() const;
    virtual const Matrix<N + 1, float> getViewToClipMatrix() const;
    virtual const Matrix<N + 1, float> getClipToIndexMatrix() const;
    virtual const Matrix<N + 1, float> getClipToDataMatrix() const;
    virtual const Matrix<N + 1, float> getClipToModelMatrix() const;
    virtual const Matrix<N + 1, float> getClipToWorldMatrix() const;
    virtual const Matrix<N + 1, float> getClipToViewMatrix() const;

private:
    const StructuredGridEntity<N>* structuredGridEntity_;
    const Camera<N>* camera_;
};

template <unsigned int N>
class VolumeCoordinateTransformerImpl : public VolumeCoordinateTransformer<N> {
public:
    VolumeCoordinateTransformerImpl(const StructuredGridEntity<N>* structuredGridEntity);
    VolumeCoordinateTransformerImpl(const VolumeCoordinateTransformerImpl<N>& rhs);
    VolumeCoordinateTransformerImpl<N>& operator=(const VolumeCoordinateTransformerImpl<N>& that);
    virtual VolumeCoordinateTransformerImpl<N>* clone() const;
    virtual ~VolumeCoordinateTransformerImpl() {}

    virtual const Matrix<N + 1, float> getIndexMatrix() const;
    virtual const Matrix<N + 1, float> getModelMatrix() const;
    virtual const Matrix<N + 1, float> getWorldMatrix() const;

    virtual const Matrix<N + 1, float> getIndexToTextureMatrix() const;
    virtual const Matrix<N + 1, float> getIndexToModelMatrix() const;
    virtual const Matrix<N + 1, float> getIndexToWorldMatrix() const;
    virtual const Matrix<N + 1, float> getTextureToIndexMatrix() const;
    virtual const Matrix<N + 1, float> getTextureToModelMatrix() const;
    virtual const Matrix<N + 1, float> getTextureToWorldMatrix() const;
    virtual const Matrix<N + 1, float> getModelToIndexMatrix() const;
    virtual const Matrix<N + 1, float> getModelToTextureMatrix() const;
    virtual const Matrix<N + 1, float> getModelToWorldMatrix() const;
    virtual const Matrix<N + 1, float> getWorldToIndexMatrix() const;
    virtual const Matrix<N + 1, float> getWorldToTextureMatrix() const;
    virtual const Matrix<N + 1, float> getWorldToModelMatrix() const;

private:
    const StructuredGridEntity<N>* structuredGridEntity_;
};

template <unsigned int N>
class VolumeCameraCoordinateTransformerImpl : public VolumeCameraCoordinateTransformer<N> {
public:
    VolumeCameraCoordinateTransformerImpl(const StructuredGridEntity<N>* structuredGridEntity,
                                          const Camera<N>* camera);
    VolumeCameraCoordinateTransformerImpl(const VolumeCameraCoordinateTransformerImpl<N>& rhs);
    VolumeCameraCoordinateTransformerImpl<N>& operator=(
        const VolumeCameraCoordinateTransformerImpl<N>& that);
    virtual VolumeCameraCoordinateTransformerImpl<N>* clone() const;
    virtual ~VolumeCameraCoordinateTransformerImpl() {}

    virtual const Matrix<N + 1, float> getIndexMatrix() const;
    virtual const Matrix<N + 1, float> getModelMatrix() const;
    virtual const Matrix<N + 1, float> getWorldMatrix() const;
    virtual const Matrix<N + 1, float> getViewMatrix() const;
    virtual const Matrix<N + 1, float> getProjectionMatrix() const;

    virtual const Matrix<N + 1, float> getIndexToTextureMatrix() const;
    virtual const Matrix<N + 1, float> getIndexToModelMatrix() const;
    virtual const Matrix<N + 1, float> getIndexToWorldMatrix() const;
    virtual const Matrix<N + 1, float> getIndexToViewMatrix() const;
    virtual const Matrix<N + 1, float> getIndexToClipMatrix() const;
    virtual const Matrix<N + 1, float> getTextureToIndexMatrix() const;
    virtual const Matrix<N + 1, float> getTextureToModelMatrix() const;
    virtual const Matrix<N + 1, float> getTextureToWorldMatrix() const;
    virtual const Matrix<N + 1, float> getTextureToViewMatrix() const;
    virtual const Matrix<N + 1, float> getTextureToClipMatrix() const;
    virtual const Matrix<N + 1, float> getModelToIndexMatrix() const;
    virtual const Matrix<N + 1, float> getModelToTextureMatrix() const;
    virtual const Matrix<N + 1, float> getModelToWorldMatrix() const;
    virtual const Matrix<N + 1, float> getModelToViewMatrix() const;
    virtual const Matrix<N + 1, float> getModelToClipMatrix() const;
    virtual const Matrix<N + 1, float> getWorldToIndexMatrix() const;
    virtual const Matrix<N + 1, float> getWorldToTextureMatrix() const;
    virtual const Matrix<N + 1, float> getWorldToModelMatrix() const;
    virtual const Matrix<N + 1, float> getWorldToViewMatrix() const;
    virtual const Matrix<N + 1, float> getWorldToClipMatrix() const;
    virtual const Matrix<N + 1, float> getViewToIndexMatrix() const;
    virtual const Matrix<N + 1, float> getViewToTextureMatrix() const;
    virtual const Matrix<N + 1, float> getViewToModelMatrix() const;
    virtual const Matrix<N + 1, float> getViewToWorldMatrix() const;
    virtual const Matrix<N + 1, float> getViewToClipMatrix() const;
    virtual const Matrix<N + 1, float> getClipToIndexMatrix() const;
    virtual const Matrix<N + 1, float> getClipToTextureMatrix() const;
    virtual const Matrix<N + 1, float> getClipToModelMatrix() const;
    virtual const Matrix<N + 1, float> getClipToWorldMatrix() const;
    virtual const Matrix<N + 1, float> getClipToViewMatrix() const;

private:
    const StructuredGridEntity<N>* structuredGridEntity_;
    const Camera<N>* camera_;
};

/*---------------------------------------------------------------*
 *  Implementations                                              *
 *  SpatialEntity                                                *
 *---------------------------------------------------------------*/

template <unsigned int N>
SpatialEntity<N>::SpatialEntity()
    : transformer_(new SpatialCoordinateTransformerImpl<N>(this))
    , modelMatrix_(1.0f)
    , worldMatrix_(1.0f) {}

template <unsigned int N>
SpatialEntity<N>::SpatialEntity(const SpatialEntity<N>& rhs)
    : transformer_(rhs.transformer_->clone())
    , modelMatrix_(rhs.modelMatrix_)
    , worldMatrix_(rhs.worldMatrix_) {}

template <unsigned int N>
SpatialEntity<N>::SpatialEntity(const Matrix<N + 1, float>& modelMatrix)
    : transformer_(new SpatialCoordinateTransformerImpl<N>(this))
    , modelMatrix_(modelMatrix)
    , worldMatrix_(1.0f) {}

template <unsigned int N>
SpatialEntity<N>::SpatialEntity(const Matrix<N + 1, float>& modelMatrix,
                                const Matrix<N + 1, float>& worldMatrix)
    : transformer_(new SpatialCoordinateTransformerImpl<N>(this))
    , modelMatrix_(modelMatrix)
    , worldMatrix_(worldMatrix) {}

template <unsigned int N>
SpatialEntity<N>::SpatialEntity(SpatialCoordinateTransformer<N>* transformer)
    : transformer_(transformer), modelMatrix_(1.0f), worldMatrix_(1.0f) {}

template <unsigned int N>
SpatialEntity<N>::SpatialEntity(SpatialCoordinateTransformer<N>* transformer,
                                const Matrix<N + 1, float>& modelMatrix)
    : transformer_(transformer), modelMatrix_(modelMatrix), worldMatrix_(1.0f) {}

template <unsigned int N>
SpatialEntity<N>::SpatialEntity(SpatialCoordinateTransformer<N>* transformer,
                                const Matrix<N + 1, float>& modelMatrix,
                                const Matrix<N + 1, float>& worldMatrix)
    : transformer_(transformer), modelMatrix_(modelMatrix), worldMatrix_(worldMatrix) {}

template <unsigned int N>
SpatialEntity<N>& SpatialEntity<N>::operator=(const SpatialEntity<N>& that) {
    if (this != &that) {
        modelMatrix_ = that.modelMatrix_;
        worldMatrix_ = that.worldMatrix_;

        if (transformer_) delete transformer_;

        transformer_ = that.transformer_->clone();
    }
    return *this;
}

template <unsigned int N>
SpatialEntity<N>::~SpatialEntity() {
    if (transformer_) delete transformer_;
}

template <unsigned int N>
Vector<N, float> SpatialEntity<N>::getOffset() const {
    Vector<N, float> offset(0.0f);

    for (int i = 0; i < N; i++) {
        offset[i] = modelMatrix_[N][i];
    }

    return offset;
}
template <unsigned int N>
void SpatialEntity<N>::setOffset(const Vector<N, float>& offset) {
    for (int i = 0; i < N; i++) {
        modelMatrix_[N][i] = offset[i];
    }
}

template <unsigned int N>
Matrix<N, float> SpatialEntity<N>::getBasis() const {
    Matrix<N, float> basis(1.0f);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            basis[i][j] = modelMatrix_[i][j];
        }
    }
    return basis;
}

template <unsigned int N>
void SpatialEntity<N>::setBasis(const Matrix<N, float>& basis) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            modelMatrix_[i][j] = basis[i][j];
        }
    }
}

template <unsigned int N>
Matrix<N + 1, float> SpatialEntity<N>::getModelMatrix() const {
    return modelMatrix_;
}

template <unsigned int N>
void SpatialEntity<N>::setModelMatrix(const Matrix<N + 1, float>& modelMatrix) {
    modelMatrix_ = modelMatrix;
}

template <unsigned int N>
Matrix<N + 1, float> SpatialEntity<N>::getWorldMatrix() const {
    return worldMatrix_;
}
template <unsigned int N>
void SpatialEntity<N>::setWorldMatrix(const Matrix<N + 1, float>& worldMatrix) {
    worldMatrix_ = worldMatrix;
}

template <unsigned int N>
const SpatialCoordinateTransformer<N>& SpatialEntity<N>::getCoordinateTransformer() const {
    return *transformer_;
}

/*---------------------------------------------------------------*
 *  Implementations                                              *
 *  StructuredGridEntity                                         *
 *---------------------------------------------------------------*/

template <unsigned int N>
StructuredGridEntity<N>::StructuredGridEntity()
    : SpatialEntity<N>(new StructuredCoordinateTransformerImpl<N>(this)), dimension_(1) {}

template <unsigned int N>
StructuredGridEntity<N>::StructuredGridEntity(const StructuredGridEntity<N>& rhs)
    : SpatialEntity<N>(rhs), dimension_(rhs.dimension_) {}

template <unsigned int N>
StructuredGridEntity<N>::StructuredGridEntity(const Vector<N, unsigned int>& dimension)
    : SpatialEntity<N>(new StructuredCoordinateTransformerImpl<N>(this)), dimension_(dimension) {}

template <unsigned int N>
StructuredGridEntity<N>::StructuredGridEntity(const Vector<N, unsigned int>& dimension,
                                              const Vector<N, float>& spacing)
    : SpatialEntity<N>(new StructuredCoordinateTransformer<N>(this)), dimension_(dimension) {
    Matrix<N, float> basis(dimension * spacing);
    setBasis(basis);
    Vector<N, float> offset(0.0f);
    for (int i = 0; i < N; ++i) {
        offset += basis[i];
    }
    setOffset(-0.5 * offset);
}

template <unsigned int N>
StructuredGridEntity<N>::StructuredGridEntity(const Vector<N, unsigned int>& dimension,
                                              const Matrix<N + 1, float>& modelMatrix)
    : SpatialEntity<N>(new StructuredCoordinateTransformer<N>(this), modelMatrix)
    , dimension_(dimension) {}

template <unsigned int N>
StructuredGridEntity<N>::StructuredGridEntity(const Vector<N, unsigned int>& dimension,
                                              const Matrix<N + 1, float>& modelMatrix,
                                              const Matrix<N + 1, float>& worldMatrix)
    : SpatialEntity<N>(new StructuredCoordinateTransformer<N>(this), modelMatrix, worldMatrix)
    , dimension_(dimension) {}

template <unsigned int N>
StructuredGridEntity<N>& StructuredGridEntity<N>::operator=(const StructuredGridEntity<N>& that) {
    if (this != &that) {
        SpatialEntity<N>::operator=(that);
        dimension_ = that.dimension_;
    }
    return *this;
}

template <unsigned int N>
Vector<N, unsigned int> StructuredGridEntity<N>::getDimension() const {
    return dimension_;
}
template <unsigned int N>
void StructuredGridEntity<N>::setDimension(const Vector<N, unsigned int>& dimension) {
    dimension_ = dimension;
}

template <unsigned int N>
Matrix<N + 1, float> StructuredGridEntity<N>::getIndexMatrix() const {
    Matrix<N + 1, float> indexMatrix(1.0f);
    for (int i = 0; i < N; ++i) {
        indexMatrix[i][i] = dimension_[i];
    }
    return indexMatrix;
}

template <unsigned int N>
const StructuredCoordinateTransformer<N>& StructuredGridEntity<N>::getCoordinateTransformer()
    const {
    return *(static_cast<StructuredCoordinateTransformer<N>*>(this->transformer_));
}

/*********************************************************************************
 *  Implementations
 *  SpatialCoordinateTransformerImpl
 *********************************************************************************/

template <unsigned int N>
SpatialCoordinateTransformerImpl<N>::SpatialCoordinateTransformerImpl(
    const SpatialEntity<N>* spatialEntity)
    : SpatialCoordinateTransformer<N>(), spatialEntity_(spatialEntity) {}

template <unsigned int N>
SpatialCoordinateTransformerImpl<N>::SpatialCoordinateTransformerImpl(
    const SpatialCoordinateTransformerImpl<N>& rhs)
    : SpatialCoordinateTransformer<N>(rhs), spatialEntity_(rhs.spatialEntity_) {}

template <unsigned int N>
SpatialCoordinateTransformerImpl<N>& SpatialCoordinateTransformerImpl<N>::operator=(
    const SpatialCoordinateTransformerImpl<N>& that) {
    if (this != &that) {
        SpatialCoordinateTransformer<N>::operator=(that);
        spatialEntity_ = that.spatialEntity_;
    }
    return *this;
}

template <unsigned int N>
SpatialCoordinateTransformerImpl<N>* SpatialCoordinateTransformerImpl<N>::clone() const {
    return new SpatialCoordinateTransformerImpl<N>(*this);
}

template <unsigned int N>
const Matrix<N + 1, float> SpatialCoordinateTransformerImpl<N>::getModelMatrix() const {
    return spatialEntity_->getModelMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> SpatialCoordinateTransformerImpl<N>::getWorldMatrix() const {
    return spatialEntity_->getWorldMatrix();
}

template <unsigned int N>
const Matrix<N + 1, float> SpatialCoordinateTransformerImpl<N>::getDataToModelMatrix() const {
    return getModelMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> SpatialCoordinateTransformerImpl<N>::getDataToWorldMatrix() const {
    return getWorldMatrix() * getModelMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> SpatialCoordinateTransformerImpl<N>::getModelToDataMatrix() const {
    return MatrixInvert(getModelMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> SpatialCoordinateTransformerImpl<N>::getModelToWorldMatrix() const {
    return getWorldMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> SpatialCoordinateTransformerImpl<N>::getWorldToDataMatrix() const {
    return MatrixInvert(getWorldMatrix() * getModelMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> SpatialCoordinateTransformerImpl<N>::getWorldToModelMatrix() const {
    return MatrixInvert(getWorldMatrix());
}

/*********************************************************************************
 *  Implementations
 *  StructuredCoordinateTransformerImpl
 *********************************************************************************/

template <unsigned int N>
StructuredCoordinateTransformerImpl<N>::StructuredCoordinateTransformerImpl(
    const StructuredGridEntity<N>* structuredGridEntity)
    : StructuredCoordinateTransformer<N>(), structuredGridEntity_(structuredGridEntity) {}

template <unsigned int N>
StructuredCoordinateTransformerImpl<N>::StructuredCoordinateTransformerImpl(
    const StructuredCoordinateTransformerImpl<N>& rhs)
    : StructuredCoordinateTransformer<N>(rhs), structuredGridEntity_(rhs.structuredGridEntity_) {}

template <unsigned int N>
StructuredCoordinateTransformerImpl<N>& StructuredCoordinateTransformerImpl<N>::operator=(
    const StructuredCoordinateTransformerImpl<N>& that) {
    if (this != &that) {
        StructuredCoordinateTransformer<N>::operator=(that);
        structuredGridEntity_ = that.structuredGridEntity_;
    }
    return *this;
}

template <unsigned int N>
StructuredCoordinateTransformerImpl<N>* StructuredCoordinateTransformerImpl<N>::clone() const {
    return new StructuredCoordinateTransformerImpl<N>(*this);
}

template <unsigned int N>
const Matrix<N + 1, float> StructuredCoordinateTransformerImpl<N>::getIndexMatrix() const {
    return structuredGridEntity_->getIndexMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> StructuredCoordinateTransformerImpl<N>::getModelMatrix() const {
    return structuredGridEntity_->getModelMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> StructuredCoordinateTransformerImpl<N>::getWorldMatrix() const {
    return structuredGridEntity_->getWorldMatrix();
}

template <unsigned int N>
const Matrix<N + 1, float> StructuredCoordinateTransformerImpl<N>::getIndexToDataMatrix() const {
    return MatrixInvert(getIndexMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> StructuredCoordinateTransformerImpl<N>::getIndexToModelMatrix() const {
    return getModelMatrix() * MatrixInvert(getIndexMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> StructuredCoordinateTransformerImpl<N>::getIndexToWorldMatrix() const {
    return getWorldMatrix() * getModelMatrix() * MatrixInvert(getIndexMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> StructuredCoordinateTransformerImpl<N>::getDataToIndexMatrix() const {
    return getIndexMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> StructuredCoordinateTransformerImpl<N>::getDataToModelMatrix() const {
    return getModelMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> StructuredCoordinateTransformerImpl<N>::getDataToWorldMatrix() const {
    return getWorldMatrix() * getModelMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> StructuredCoordinateTransformerImpl<N>::getModelToIndexMatrix() const {
    return getIndexMatrix() * MatrixInvert(getModelMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> StructuredCoordinateTransformerImpl<N>::getModelToDataMatrix() const {
    return MatrixInvert(getModelMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> StructuredCoordinateTransformerImpl<N>::getModelToWorldMatrix() const {
    return getWorldMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> StructuredCoordinateTransformerImpl<N>::getWorldToIndexMatrix() const {
    return getIndexMatrix() * MatrixInvert(getWorldMatrix() * getModelMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> StructuredCoordinateTransformerImpl<N>::getWorldToDataMatrix() const {
    return MatrixInvert(getWorldMatrix() * getModelMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> StructuredCoordinateTransformerImpl<N>::getWorldToModelMatrix() const {
    return MatrixInvert(getWorldMatrix());
}

/*********************************************************************************
 *  Implementations
 *  SpatialCameraCoordinateTransformerImpl
 *********************************************************************************/

template <unsigned int N>
SpatialCameraCoordinateTransformerImpl<N>::SpatialCameraCoordinateTransformerImpl(
    const SpatialEntity<N>* spatialEntity, const Camera<N>* camera)
    : SpatialCameraCoordinateTransformer<N>(), spatialEntity_(spatialEntity), camera_(camera) {}

template <unsigned int N>
SpatialCameraCoordinateTransformerImpl<N>::SpatialCameraCoordinateTransformerImpl(
    const SpatialCameraCoordinateTransformerImpl<N>& rhs)
    : SpatialCameraCoordinateTransformer<N>(rhs)
    , spatialEntity_(rhs.spatialEntity_)
    , camera_(rhs.camera_) {}

template <unsigned int N>
SpatialCameraCoordinateTransformerImpl<N>& SpatialCameraCoordinateTransformerImpl<N>::operator=(
    const SpatialCameraCoordinateTransformerImpl<N>& that) {
    if (this != &that) {
        SpatialCameraCoordinateTransformer<N>::operator=(that);
        spatialEntity_ = that.spatialEntity_;
        camera_ = that.camera_;
    }
    return *this;
}

template <unsigned int N>
SpatialCameraCoordinateTransformerImpl<N>* SpatialCameraCoordinateTransformerImpl<N>::clone()
    const {
    return new SpatialCameraCoordinateTransformerImpl<N>(*this);
}

template <unsigned int N>
const Matrix<N + 1, float> SpatialCameraCoordinateTransformerImpl<N>::getModelMatrix() const {
    return spatialEntity_->getModelMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> SpatialCameraCoordinateTransformerImpl<N>::getWorldMatrix() const {
    return spatialEntity_->getWorldMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> SpatialCameraCoordinateTransformerImpl<N>::getViewMatrix() const {
    return camera_->getViewMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> SpatialCameraCoordinateTransformerImpl<N>::getProjectionMatrix() const {
    return camera_->getProjectionMatrix();
}

template <unsigned int N>
const Matrix<N + 1, float> SpatialCameraCoordinateTransformerImpl<N>::getDataToModelMatrix() const {
    return getModelMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> SpatialCameraCoordinateTransformerImpl<N>::getDataToWorldMatrix() const {
    return getWorldMatrix() * getModelMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> SpatialCameraCoordinateTransformerImpl<N>::getDataToViewMatrix() const {
    return getViewMatrix() * getWorldMatrix() * getModelMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> SpatialCameraCoordinateTransformerImpl<N>::getDataToClipMatrix() const {
    return getProjectionMatrix() * getViewMatrix() * getWorldMatrix() * getModelMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> SpatialCameraCoordinateTransformerImpl<N>::getModelToDataMatrix() const {
    return MatrixInvert(getModelMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> SpatialCameraCoordinateTransformerImpl<N>::getModelToWorldMatrix()
    const {
    return getWorldMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> SpatialCameraCoordinateTransformerImpl<N>::getModelToViewMatrix() const {
    return getViewMatrix() * getWorldMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> SpatialCameraCoordinateTransformerImpl<N>::getModelToClipMatrix() const {
    return getProjectionMatrix() * getViewMatrix() * getWorldMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> SpatialCameraCoordinateTransformerImpl<N>::getWorldToDataMatrix() const {
    return MatrixInvert(getWorldMatrix() * getModelMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> SpatialCameraCoordinateTransformerImpl<N>::getWorldToModelMatrix()
    const {
    return MatrixInvert(getWorldMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> SpatialCameraCoordinateTransformerImpl<N>::getWorldToViewMatrix() const {
    return getViewMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> SpatialCameraCoordinateTransformerImpl<N>::getWorldToClipMatrix() const {
    return getProjectionMatrix() * getViewMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> SpatialCameraCoordinateTransformerImpl<N>::getViewToDataMatrix() const {
    return MatrixInvert(getViewMatrix() * getWorldMatrix() * getModelMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> SpatialCameraCoordinateTransformerImpl<N>::getViewToModelMatrix() const {
    return MatrixInvert(getViewMatrix() * getWorldMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> SpatialCameraCoordinateTransformerImpl<N>::getViewToWorldMatrix() const {
    return MatrixInvert(getViewMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> SpatialCameraCoordinateTransformerImpl<N>::getViewToClipMatrix() const {
    return getProjectionMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> SpatialCameraCoordinateTransformerImpl<N>::getClipToDataMatrix() const {
    return MatrixInvert(getProjectionMatrix() * getViewMatrix() * getWorldMatrix() *
                        getModelMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> SpatialCameraCoordinateTransformerImpl<N>::getClipToModelMatrix() const {
    return MatrixInvert(getProjectionMatrix() * getViewMatrix() * getWorldMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> SpatialCameraCoordinateTransformerImpl<N>::getClipToWorldMatrix() const {
    return MatrixInvert(getProjectionMatrix() * getViewMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> SpatialCameraCoordinateTransformerImpl<N>::getClipToViewMatrix() const {
    return MatrixInvert(getProjectionMatrix());
}

/*********************************************************************************
 *  Implementations
 *  StructuedCameraCoordinateTransformerImpl
 *********************************************************************************/

template <unsigned int N>
StructuedCameraCoordinateTransformerImpl<N>::StructuedCameraCoordinateTransformerImpl(
    const StructuredGridEntity<N>* structuredGridEntity, const Camera<N>* camera)
    : StructuedCameraCoordinateTransformer<N>()
    , structuredGridEntity_(structuredGridEntity)
    , camera_(camera) {}

template <unsigned int N>
StructuedCameraCoordinateTransformerImpl<N>::StructuedCameraCoordinateTransformerImpl(
    const StructuedCameraCoordinateTransformerImpl<N>& rhs)
    : StructuedCameraCoordinateTransformer<N>(rhs)
    , structuredGridEntity_(rhs.structuredGridEntity_)
    , camera_(rhs.camera_) {}

template <unsigned int N>
StructuedCameraCoordinateTransformerImpl<N>& StructuedCameraCoordinateTransformerImpl<N>::operator=(
    const StructuedCameraCoordinateTransformerImpl<N>& that) {
    if (this != &that) {
        StructuedCameraCoordinateTransformer<N>::operator=(that);
        structuredGridEntity_ = that.structuredGridEntity_;
        camera_ = that.camera_;
    }
    return *this;
}

template <unsigned int N>
StructuedCameraCoordinateTransformerImpl<N>* StructuedCameraCoordinateTransformerImpl<N>::clone()
    const {
    return new StructuedCameraCoordinateTransformerImpl<N>(*this);
}

template <unsigned int N>
const Matrix<N + 1, float> StructuedCameraCoordinateTransformerImpl<N>::getIndexMatrix() const {
    return structuredGridEntity_->getIndexMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> StructuedCameraCoordinateTransformerImpl<N>::getModelMatrix() const {
    return structuredGridEntity_->getModelMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> StructuedCameraCoordinateTransformerImpl<N>::getWorldMatrix() const {
    return structuredGridEntity_->getWorldMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> StructuedCameraCoordinateTransformerImpl<N>::getViewMatrix() const {
    return camera_->getViewMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> StructuedCameraCoordinateTransformerImpl<N>::getProjectionMatrix()
    const {
    return camera_->getProjectionMatrix();
}

template <unsigned int N>
const Matrix<N + 1, float> StructuedCameraCoordinateTransformerImpl<N>::getIndexToDataMatrix()
    const {
    return MatrixInvert(getIndexMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> StructuedCameraCoordinateTransformerImpl<N>::getIndexToModelMatrix()
    const {
    return getModelMatrix() * MatrixInvert(getIndexMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> StructuedCameraCoordinateTransformerImpl<N>::getIndexToWorldMatrix()
    const {
    return getWorldMatrix() * getModelMatrix() * MatrixInvert(getIndexMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> StructuedCameraCoordinateTransformerImpl<N>::getIndexToViewMatrix()
    const {
    return getViewMatrix() * getWorldMatrix() * getModelMatrix() * MatrixInvert(getIndexMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> StructuedCameraCoordinateTransformerImpl<N>::getIndexToClipMatrix()
    const {
    return getProjectionMatrix() * getViewMatrix() * getWorldMatrix() * getModelMatrix() *
           MatrixInvert(getIndexMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> StructuedCameraCoordinateTransformerImpl<N>::getDataToIndexMatrix()
    const {
    return getIndexMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> StructuedCameraCoordinateTransformerImpl<N>::getDataToModelMatrix()
    const {
    return getModelMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> StructuedCameraCoordinateTransformerImpl<N>::getDataToWorldMatrix()
    const {
    return getWorldMatrix() * getModelMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> StructuedCameraCoordinateTransformerImpl<N>::getDataToViewMatrix()
    const {
    return getViewMatrix() * getWorldMatrix() * getModelMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> StructuedCameraCoordinateTransformerImpl<N>::getDataToClipMatrix()
    const {
    return getProjectionMatrix() * getViewMatrix() * getWorldMatrix() * getModelMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> StructuedCameraCoordinateTransformerImpl<N>::getModelToIndexMatrix()
    const {
    return getIndexMatrix() * MatrixInvert(getModelMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> StructuedCameraCoordinateTransformerImpl<N>::getModelToDataMatrix()
    const {
    return MatrixInvert(getModelMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> StructuedCameraCoordinateTransformerImpl<N>::getModelToWorldMatrix()
    const {
    return getWorldMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> StructuedCameraCoordinateTransformerImpl<N>::getModelToViewMatrix()
    const {
    return getViewMatrix() * getWorldMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> StructuedCameraCoordinateTransformerImpl<N>::getModelToClipMatrix()
    const {
    return getProjectionMatrix() * getViewMatrix() * getWorldMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> StructuedCameraCoordinateTransformerImpl<N>::getWorldToIndexMatrix()
    const {
    return getIndexMatrix() * MatrixInvert(getWorldMatrix() * getModelMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> StructuedCameraCoordinateTransformerImpl<N>::getWorldToDataMatrix()
    const {
    return MatrixInvert(getWorldMatrix() * getModelMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> StructuedCameraCoordinateTransformerImpl<N>::getWorldToModelMatrix()
    const {
    return MatrixInvert(getWorldMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> StructuedCameraCoordinateTransformerImpl<N>::getWorldToViewMatrix()
    const {
    return getViewMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> StructuedCameraCoordinateTransformerImpl<N>::getWorldToClipMatrix()
    const {
    return getProjectionMatrix() * getViewMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> StructuedCameraCoordinateTransformerImpl<N>::getViewToIndexMatrix()
    const {
    return getIndexMatrix() * MatrixInvert(getViewMatrix() * getWorldMatrix() * getModelMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> StructuedCameraCoordinateTransformerImpl<N>::getViewToDataMatrix()
    const {
    return MatrixInvert(getViewMatrix() * getWorldMatrix() * getModelMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> StructuedCameraCoordinateTransformerImpl<N>::getViewToModelMatrix()
    const {
    return MatrixInvert(getViewMatrix() * getWorldMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> StructuedCameraCoordinateTransformerImpl<N>::getViewToWorldMatrix()
    const {
    return MatrixInvert(getViewMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> StructuedCameraCoordinateTransformerImpl<N>::getViewToClipMatrix()
    const {
    return getProjectionMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> StructuedCameraCoordinateTransformerImpl<N>::getClipToIndexMatrix()
    const {
    return getIndexMatrix() * MatrixInvert(getProjectionMatrix() * getViewMatrix() *
                                           getWorldMatrix() * getModelMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> StructuedCameraCoordinateTransformerImpl<N>::getClipToDataMatrix()
    const {
    return MatrixInvert(getProjectionMatrix() * getViewMatrix() * getWorldMatrix() *
                        getModelMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> StructuedCameraCoordinateTransformerImpl<N>::getClipToModelMatrix()
    const {
    return MatrixInvert(getProjectionMatrix() * getViewMatrix() * getWorldMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> StructuedCameraCoordinateTransformerImpl<N>::getClipToWorldMatrix()
    const {
    return MatrixInvert(getProjectionMatrix() * getViewMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> StructuedCameraCoordinateTransformerImpl<N>::getClipToViewMatrix()
    const {
    return MatrixInvert(getProjectionMatrix());
}

/*********************************************************************************
 *  Implementations
 *  VolumeCoordinateTransformerImpl
 *********************************************************************************/

template <unsigned int N>
VolumeCoordinateTransformerImpl<N>::VolumeCoordinateTransformerImpl(
    const StructuredGridEntity<N>* structuredGridEntity)
    : VolumeCoordinateTransformer<N>(), structuredGridEntity_(structuredGridEntity) {}

template <unsigned int N>
VolumeCoordinateTransformerImpl<N>::VolumeCoordinateTransformerImpl(
    const VolumeCoordinateTransformerImpl<N>& rhs)
    : VolumeCoordinateTransformer<N>(rhs), structuredGridEntity_(rhs.structuredGridEntity_) {}

template <unsigned int N>
VolumeCoordinateTransformerImpl<N>& VolumeCoordinateTransformerImpl<N>::operator=(
    const VolumeCoordinateTransformerImpl<N>& that) {
    if (this != &that) {
        VolumeCoordinateTransformer<N>::operator=(that);
        structuredGridEntity_ = that.structuredGridEntity_;
    }
    return *this;
}

template <unsigned int N>
VolumeCoordinateTransformerImpl<N>* VolumeCoordinateTransformerImpl<N>::clone() const {
    return new VolumeCoordinateTransformerImpl<N>(*this);
}

template <unsigned int N>
const Matrix<N + 1, float> VolumeCoordinateTransformerImpl<N>::getIndexMatrix() const {
    return structuredGridEntity_->getIndexMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCoordinateTransformerImpl<N>::getModelMatrix() const {
    return structuredGridEntity_->getModelMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCoordinateTransformerImpl<N>::getWorldMatrix() const {
    return structuredGridEntity_->getWorldMatrix();
}

template <unsigned int N>
const Matrix<N + 1, float> VolumeCoordinateTransformerImpl<N>::getIndexToTextureMatrix() const {
    return MatrixInvert(getIndexMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCoordinateTransformerImpl<N>::getIndexToModelMatrix() const {
    return getModelMatrix() * MatrixInvert(getIndexMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCoordinateTransformerImpl<N>::getIndexToWorldMatrix() const {
    return getWorldMatrix() * getModelMatrix() * MatrixInvert(getIndexMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCoordinateTransformerImpl<N>::getTextureToIndexMatrix() const {
    return getIndexMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCoordinateTransformerImpl<N>::getTextureToModelMatrix() const {
    return getModelMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCoordinateTransformerImpl<N>::getTextureToWorldMatrix() const {
    return getWorldMatrix() * getModelMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCoordinateTransformerImpl<N>::getModelToIndexMatrix() const {
    return getIndexMatrix() * MatrixInvert(getModelMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCoordinateTransformerImpl<N>::getModelToTextureMatrix() const {
    return MatrixInvert(getModelMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCoordinateTransformerImpl<N>::getModelToWorldMatrix() const {
    return getWorldMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCoordinateTransformerImpl<N>::getWorldToIndexMatrix() const {
    return getIndexMatrix() * MatrixInvert(getWorldMatrix() * getModelMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCoordinateTransformerImpl<N>::getWorldToTextureMatrix() const {
    return MatrixInvert(getWorldMatrix() * getModelMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCoordinateTransformerImpl<N>::getWorldToModelMatrix() const {
    return MatrixInvert(getWorldMatrix());
}

/*********************************************************************************
 *  Implementations
 *  VolumeCameraCoordinateTransformerImpl
 *********************************************************************************/

template <unsigned int N>
VolumeCameraCoordinateTransformerImpl<N>::VolumeCameraCoordinateTransformerImpl(
    const StructuredGridEntity<N>* structuredGridEntity, const Camera<N>* camera)
    : VolumeCameraCoordinateTransformer<N>()
    , structuredGridEntity_(structuredGridEntity)
    , camera_(camera) {}

template <unsigned int N>
VolumeCameraCoordinateTransformerImpl<N>::VolumeCameraCoordinateTransformerImpl(
    const VolumeCameraCoordinateTransformerImpl<N>& rhs)
    : VolumeCameraCoordinateTransformer<N>(rhs)
    , structuredGridEntity_(rhs.structuredGridEntity_)
    , camera_(rhs.camera_) {}

template <unsigned int N>
VolumeCameraCoordinateTransformerImpl<N>& VolumeCameraCoordinateTransformerImpl<N>::operator=(
    const VolumeCameraCoordinateTransformerImpl<N>& that) {
    if (this != &that) {
        VolumeCameraCoordinateTransformer<N>::operator=(that);
        structuredGridEntity_ = that.structuredGridEntity_;
        camera_ = that.camera_;
    }
    return *this;
}

template <unsigned int N>
VolumeCameraCoordinateTransformerImpl<N>* VolumeCameraCoordinateTransformerImpl<N>::clone() const {
    return new VolumeCameraCoordinateTransformerImpl<N>(*this);
}

template <unsigned int N>
const Matrix<N + 1, float> VolumeCameraCoordinateTransformerImpl<N>::getIndexMatrix() const {
    return structuredGridEntity_->getIndexMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCameraCoordinateTransformerImpl<N>::getModelMatrix() const {
    return structuredGridEntity_->getModelMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCameraCoordinateTransformerImpl<N>::getWorldMatrix() const {
    return structuredGridEntity_->getWorldMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCameraCoordinateTransformerImpl<N>::getViewMatrix() const {
    return camera_->getViewMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCameraCoordinateTransformerImpl<N>::getProjectionMatrix() const {
    return camera_->getProjectionMatrix();
}

template <unsigned int N>
const Matrix<N + 1, float> VolumeCameraCoordinateTransformerImpl<N>::getIndexToTextureMatrix()
    const {
    return MatrixInvert(getIndexMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCameraCoordinateTransformerImpl<N>::getIndexToModelMatrix() const {
    return getModelMatrix() * MatrixInvert(getIndexMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCameraCoordinateTransformerImpl<N>::getIndexToWorldMatrix() const {
    return getWorldMatrix() * getModelMatrix() * MatrixInvert(getIndexMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCameraCoordinateTransformerImpl<N>::getIndexToViewMatrix() const {
    return getViewMatrix() * getWorldMatrix() * getModelMatrix() * MatrixInvert(getIndexMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCameraCoordinateTransformerImpl<N>::getIndexToClipMatrix() const {
    return getProjectionMatrix() * getViewMatrix() * getWorldMatrix() * getModelMatrix() *
           MatrixInvert(getIndexMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCameraCoordinateTransformerImpl<N>::getTextureToIndexMatrix()
    const {
    return getIndexMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCameraCoordinateTransformerImpl<N>::getTextureToModelMatrix()
    const {
    return getModelMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCameraCoordinateTransformerImpl<N>::getTextureToWorldMatrix()
    const {
    return getWorldMatrix() * getModelMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCameraCoordinateTransformerImpl<N>::getTextureToViewMatrix()
    const {
    return getViewMatrix() * getWorldMatrix() * getModelMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCameraCoordinateTransformerImpl<N>::getTextureToClipMatrix()
    const {
    return getProjectionMatrix() * getViewMatrix() * getWorldMatrix() * getModelMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCameraCoordinateTransformerImpl<N>::getModelToIndexMatrix() const {
    return getIndexMatrix() * MatrixInvert(getModelMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCameraCoordinateTransformerImpl<N>::getModelToTextureMatrix()
    const {
    return MatrixInvert(getModelMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCameraCoordinateTransformerImpl<N>::getModelToWorldMatrix() const {
    return getWorldMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCameraCoordinateTransformerImpl<N>::getModelToViewMatrix() const {
    return getViewMatrix() * getWorldMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCameraCoordinateTransformerImpl<N>::getModelToClipMatrix() const {
    return getProjectionMatrix() * getViewMatrix() * getWorldMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCameraCoordinateTransformerImpl<N>::getWorldToIndexMatrix() const {
    return getIndexMatrix() * MatrixInvert(getWorldMatrix() * getModelMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCameraCoordinateTransformerImpl<N>::getWorldToTextureMatrix()
    const {
    return MatrixInvert(getWorldMatrix() * getModelMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCameraCoordinateTransformerImpl<N>::getWorldToModelMatrix() const {
    return MatrixInvert(getWorldMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCameraCoordinateTransformerImpl<N>::getWorldToViewMatrix() const {
    return getViewMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCameraCoordinateTransformerImpl<N>::getWorldToClipMatrix() const {
    return getProjectionMatrix() * getViewMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCameraCoordinateTransformerImpl<N>::getViewToIndexMatrix() const {
    return getIndexMatrix() * MatrixInvert(getViewMatrix() * getWorldMatrix() * getModelMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCameraCoordinateTransformerImpl<N>::getViewToTextureMatrix()
    const {
    return MatrixInvert(getViewMatrix() * getWorldMatrix() * getModelMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCameraCoordinateTransformerImpl<N>::getViewToModelMatrix() const {
    return MatrixInvert(getViewMatrix() * getWorldMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCameraCoordinateTransformerImpl<N>::getViewToWorldMatrix() const {
    return MatrixInvert(getViewMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCameraCoordinateTransformerImpl<N>::getViewToClipMatrix() const {
    return getProjectionMatrix();
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCameraCoordinateTransformerImpl<N>::getClipToIndexMatrix() const {
    return getIndexMatrix() * MatrixInvert(getProjectionMatrix() * getViewMatrix() *
                                           getWorldMatrix() * getModelMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCameraCoordinateTransformerImpl<N>::getClipToTextureMatrix()
    const {
    return MatrixInvert(getProjectionMatrix() * getViewMatrix() * getWorldMatrix() *
                        getModelMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCameraCoordinateTransformerImpl<N>::getClipToModelMatrix() const {
    return MatrixInvert(getProjectionMatrix() * getViewMatrix() * getWorldMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCameraCoordinateTransformerImpl<N>::getClipToWorldMatrix() const {
    return MatrixInvert(getProjectionMatrix() * getViewMatrix());
}
template <unsigned int N>
const Matrix<N + 1, float> VolumeCameraCoordinateTransformerImpl<N>::getClipToViewMatrix() const {
    return MatrixInvert(getProjectionMatrix());
}

}  // namespace
#endif  // IVW_SPATIALDATA_H