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
 * Primary author : Peter Steneteg
 *
 **********************************************************************/

#ifndef IVW_SPATIALDATA_H
#define IVW_SPATIALDATA_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>


namespace inviwo {

/** \brief A convenience class to generate transformation matrices between  
 *         the different coordinate systems in use.
 *
 *  Spatial meta data in Inviwo uses 4 different coordinate systems, they are defined as
 *  - Index - The voxel indices in the data 
 *  - Texture - The corresponding texture coordinates of the data.
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

template<unsigned int N>
class CoordinateTransformer {
public:
    CoordinateTransformer(){};
    virtual ~CoordinateTransformer(){};
    /**
     * Returns the matrix transformation mapping from texture coordinates
     * to voxel index coordinates, i.e. from [0,1] to [0, number of voxels-1)
     */
    virtual const Matrix<N+1, float> getTextureToIndexMatrix() const = 0;

    /**
     * Returns the matrix transformation mapping from texture coordinates
     * to data model coordinates, i.e. from [0,1] to (data min, data max)
     */
    virtual const Matrix<N+1, float> getTextureToModelMatrix() const = 0;

    /**
     * Returns the matrix transformation mapping from texture coordinates
     * to opengl world coordinates, i.e. from [0,1] to (-inf, inf)
     */
    virtual const Matrix<N+1, float> getTextureToWorldMatrix() const = 0;

    /**
     * Returns the matrix transformation mapping from voxel index coordinates
     * to texture coordinates, i.e. from [0, number of voxels-1) to [0,1]
     */
    virtual const Matrix<N+1, float> getIndexToTextureMatrix() const = 0;

    /**
     * Returns the matrix transformation mapping from voxel index coordinates
     * to data model coordinates, i.e. from [0, number of voxels-1) to (data min, data max)
     */
    virtual const Matrix<N+1, float> getIndexToModelMatrix() const = 0;

    /**
     * Returns the matrix transformation mapping from voxel index coordinates
     * to opengl world coordinates, i.e. from [0, number of voxels-1) to (-inf, inf)
     */
    virtual const Matrix<N+1, float> getIndexToWorldMatrix() const = 0;

    /**
     * Returns the matrix transformation mapping from data model coordinates
     * to texture coordinates, i.e. from (data min, data max) to [0,1]
     */
    virtual const Matrix<N+1, float> getModelToTextureMatrix() const = 0;

    /**
     * Returns the matrix transformation mapping from data model coordinates
     * to voxel index coordinates, i.e. from (data min, data max) to [0, number of voxels-1)
     */
    virtual const Matrix<N+1, float> getModelToIndexMatrix() const = 0;

    /**
     * Returns the matrix transformation mapping from data model coordinates
     * to opengl world coordinates, i.e. from (data min, data max) to (-inf, inf)
     */
    virtual const Matrix<N+1, float> getModelToWorldMatrix() const = 0;

    /**
     * Returns the matrix transformation mapping from opengl world coordinates
     * to texture coordinates, i.e. from (-inf, inf) to [0,1]
     */
    virtual const Matrix<N+1, float> getWorldToTextureMatrix() const = 0;

    /**
     * Returns the matrix transformation mapping from opengl world coordinates
     * to voxel index coordinates, i.e. from (-inf, inf) to [0, number of voxels-1)
     */
    virtual const Matrix<N+1, float> getWorldToIndexMatrix() const = 0;

    /**
     * Returns the matrix transformation mapping from opengl world coordinates
     * to data model coordinates, i.e. from (-inf, inf) to (data min, data max)
     */
    virtual const Matrix<N+1, float> getWorldToModelMatrix() const = 0;
};

template <unsigned int N> class SpatialEntity;

template<unsigned int N>
class SpatialCoordinateTransformer : public CoordinateTransformer<N> {
public:
    SpatialCoordinateTransformer(const SpatialEntity<N>* spatialData) : CoordinateTransformer<N>(), spatialData_(spatialData) {};
    virtual ~SpatialCoordinateTransformer(){};

    virtual const Matrix<N+1 ,float> getWorldMatrix() const;
    virtual const Matrix<N+1, float> getBasisMatrix() const;
    virtual const Matrix<N+1, float> getDimensionMatrix() const;

    virtual const Matrix<N+1, float> getTextureToIndexMatrix() const;
    virtual const Matrix<N+1, float> getTextureToModelMatrix() const;
    virtual const Matrix<N+1, float> getTextureToWorldMatrix() const;
    virtual const Matrix<N+1, float> getIndexToTextureMatrix() const;
    virtual const Matrix<N+1, float> getIndexToModelMatrix() const;
    virtual const Matrix<N+1, float> getIndexToWorldMatrix() const;
    virtual const Matrix<N+1, float> getModelToTextureMatrix() const;
    virtual const Matrix<N+1, float> getModelToIndexMatrix() const;
    virtual const Matrix<N+1, float> getModelToWorldMatrix() const;
    virtual const Matrix<N+1, float> getWorldToTextureMatrix() const;
    virtual const Matrix<N+1, float> getWorldToIndexMatrix() const;
    virtual const Matrix<N+1, float> getWorldToModelMatrix() const;

private:
    const SpatialEntity<N>* spatialData_;
};

template <unsigned int N> class StructuredGridEntity;
template <unsigned int N>
class StructuredCoordinateTransformer : public SpatialCoordinateTransformer<N> {
public:
    StructuredCoordinateTransformer(const StructuredGridEntity<N>* structuredData) : SpatialCoordinateTransformer<N>(structuredData), structuredData_(structuredData) {};
    virtual ~StructuredCoordinateTransformer(){};

    virtual const Matrix<N+1,float> getDimensionMatrix() const {
        Vector<N,unsigned int> dim = structuredData_->getDimension();
        Matrix<N+1,float> mat(0.0f);
        for(int i=0;i<N;i++)
            mat[i][i]=(float)dim[i];
        mat[N+1][N+1]=1.0f;
        return mat;
    }

private:
    const StructuredGridEntity<N>* structuredData_;
};

             
template <unsigned int N>
class SpatialEntity {
public:
    SpatialEntity();
    SpatialEntity(const SpatialEntity<N>&);
    SpatialEntity(const Vector<N,float>& offset);
    SpatialEntity(const Matrix<N,float>& basis);
    SpatialEntity(const Matrix<N+1,float>& mat);
    SpatialEntity(const Matrix<N,float>& basis, const Vector<N,float>& offset);

    SpatialEntity<N>& operator=(const SpatialEntity<N>& that);
    virtual SpatialEntity<N>* clone() const = 0;
    virtual ~SpatialEntity();
    
    Vector<N,float> getOffset() const;
    void setOffset(const Vector<N,float>& offset);

    // Using row vectors in basis
    Matrix<N,float> getBasis() const;
    void setBasis(const Matrix<N,float>& basis);

    Matrix<N+1,float> getBasisAndOffset() const;
    void setBasisAndOffset(const Matrix<N+1,float>& mat);

    Matrix<N+1,float> getWorldTransform() const;
    void setWorldTransform(const Matrix<N+1,float>& mat);

    virtual const CoordinateTransformer<N>& getCoordinateTransformer() const;

protected:
    virtual void initTransformer();
    CoordinateTransformer<N>* transformer_;
    Matrix<N+1, float> basisAndOffset_;
    Matrix<N+1, float> worldTransform_;
};

template <unsigned int N>
class StructuredGridEntity : public SpatialEntity<N> {
public:
    StructuredGridEntity(const Vector<N, unsigned int>& dimension);
    StructuredGridEntity(const Vector<N,float>& offset, 
        const Vector<N, unsigned int>& dimension);
    StructuredGridEntity(const Matrix<N,float>& basis, 
        const Vector<N, unsigned int>& dimension);
    StructuredGridEntity(const Matrix<N,float>& basis, 
        const Vector<N,float>& offset, 
        const Vector<N, unsigned int>& dimension);

    StructuredGridEntity(const StructuredGridEntity<N>& rhs);
    StructuredGridEntity<N>& operator=(const StructuredGridEntity<N>& that);
    virtual StructuredGridEntity<N>* clone() const = 0;

    virtual ~StructuredGridEntity(){}
    

    Vector<N, unsigned int> getDimension() const;
    void setDimension(const Vector<N, unsigned int>& dimension);

protected:
    virtual void initTransformer();
    Vector<N, unsigned int> dimension_;
};


/*---------------------------------------------------------------*/
/*--Implementations----------------------------------------------*/
/*--SpatialMetaData--------------------------------------------------*/
/*---------------------------------------------------------------*/

template <unsigned int N>
SpatialEntity<N>::SpatialEntity() 
    : transformer_(NULL)
    , worldTransform_(1.0f) {
    Vector<N,float> offset(-1.0f);
    Matrix<N,float> basis(2.0f);
    setBasis(basis);
    setOffset(offset);
    initTransformer();
}

template <unsigned int N>
SpatialEntity<N>::SpatialEntity(const Vector<N,float>& offset) 
    : transformer_(NULL)
    , worldTransform_(1.0f) {
    Matrix<N,float> basis(2.0f);
    setBasis(basis);
    setOffset(offset);
    initTransformer();
}
template <unsigned int N>
SpatialEntity<N>::SpatialEntity(const Matrix<N,float>& basis) 
    : transformer_(NULL)
    , worldTransform_(1.0f) {
    Vector<N,float> offset(-1.0f);
    setBasis(basis);
    setOffset(offset);
    initTransformer();
}
template <unsigned int N>
SpatialEntity<N>::SpatialEntity(const Matrix<N+1,float>& mat) 
    : transformer_(NULL)
    , worldTransform_(1.0f) {
    setBasisAndOffset(mat);
    initTransformer();
}
template <unsigned int N>
SpatialEntity<N>::SpatialEntity(const Matrix<N,float>& basis, const Vector<N,float>& offset) 
    : transformer_(NULL)
    , worldTransform_(1.0f) {
    setBasis(basis);
    setOffset(offset);
    initTransformer();
}
template <unsigned int N>
SpatialEntity<N>::SpatialEntity(const SpatialEntity<N>& rhs)
    : transformer_(NULL)
    , basisAndOffset_(rhs.basisAndOffset_)
    , worldTransform_(rhs.worldTransform_) {
    initTransformer();
}

template <unsigned int N>
SpatialEntity<N>& SpatialEntity<N>::operator=(const SpatialEntity<N>& that) {
    if(this != &that) {
        basisAndOffset_ = that.basisAndOffset_;
        worldTransform_ = that.worldTransform_;
        if(transformer_) {
            delete transformer_;
        }
      initTransformer();
    }
    return *this;
}

template <unsigned int N>
void SpatialEntity<N>::initTransformer(){
    transformer_ = new SpatialCoordinateTransformer<N>(this);
}

template <unsigned int N>
SpatialEntity<N>::~SpatialEntity(){
    if(transformer_){
        delete transformer_;
    }
}

template<unsigned int N>
const Matrix<N+1,float> SpatialCoordinateTransformer<N>::getDimensionMatrix() const{
    Matrix<N+1,float> mat(1.0f);
    return mat;
}


template<unsigned int N>
const Matrix<N+1,float> SpatialCoordinateTransformer<N>::getBasisMatrix() const{
    return spatialData_->getBasisAndOffset();
}


template<unsigned int N>
const Matrix<N+1,float> SpatialCoordinateTransformer<N>::getWorldMatrix() const{
    return spatialData_->getWorldTransform();
}

template <unsigned int N>
Vector<N,float> SpatialEntity<N>::getOffset() const {
    Vector<N,float> offset(0.0f);
    for(int i=0;i<N;i++){
        offset[i] = basisAndOffset_[N][i];
    }
    return offset;
}
template <unsigned int N>
void SpatialEntity<N>::setOffset(const Vector<N,float>& offset) {
     for(int i=0;i<N;i++){
         basisAndOffset_[N][i] = offset[i];
     }
}

template <unsigned int N>
Matrix<N,float> SpatialEntity<N>::getBasis() const {
    Matrix<N,float> basis(1.0f);
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            basis[i][j] = basisAndOffset_[i][j];
        }
    }
    return basis;
}

template <unsigned int N>
void SpatialEntity<N>::setBasis(const Matrix<N,float>& basis) {
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            basisAndOffset_[i][j] = basis[i][j];
        }
    }
}

template <unsigned int N>
Matrix<N+1,float> SpatialEntity<N>::getBasisAndOffset() const {
    return basisAndOffset_;
}
template <unsigned int N>
void SpatialEntity<N>::setBasisAndOffset(const Matrix<N+1,float>& mat) {
    basisAndOffset_ = mat;
}

template <unsigned int N>
Matrix<N+1,float> SpatialEntity<N>::getWorldTransform() const {
   return worldTransform_;
}
template <unsigned int N>
void SpatialEntity<N>::setWorldTransform(const Matrix<N+1,float>& mat) {
    worldTransform_ = mat;
}

template <unsigned int N>
const CoordinateTransformer<N>& SpatialEntity<N>::getCoordinateTransformer() const{
    return *transformer_;
}

/*---------------------------------------------------------------*/
/*--StructuredGridMetaData-----------------------------------------------*/
/*---------------------------------------------------------------*/

template <unsigned int N>
StructuredGridEntity<N>::StructuredGridEntity(const Vector<N, unsigned int>& dimension)
    : SpatialEntity<N>()
    , dimension_(dimension) {
}

template <unsigned int N>
StructuredGridEntity<N>::StructuredGridEntity(const Vector<N,float>& offset, 
                                  const Vector<N, unsigned int>& dimension) 
    : SpatialEntity<N>(offset)
    , dimension_(dimension) {
}

template <unsigned int N>
StructuredGridEntity<N>::StructuredGridEntity(const Matrix<N,float>& basis, 
                                  const Vector<N, unsigned int>& dimension)
    : SpatialEntity<N>(basis)
    , dimension_(dimension) {
}
template <unsigned int N>
StructuredGridEntity<N>::StructuredGridEntity(const Matrix<N,float>& basis, 
                                  const Vector<N,float>& offset, 
                                  const Vector<N, unsigned int>& dimension)
    : SpatialEntity<N>(basis, offset)
    , dimension_(dimension) {
}

template <unsigned int N>
StructuredGridEntity<N>::StructuredGridEntity(const StructuredGridEntity<N>& rhs)
    : SpatialEntity<N>(rhs)
    , dimension_(rhs.dimension_) {
}

template <unsigned int N>
StructuredGridEntity<N>& StructuredGridEntity<N>::operator=(const StructuredGridEntity<N>& that) {
    if(this != &that) {
        SpatialEntity<N>::operator=(that);
        dimension_ = that.dimension_;
    }
    return *this;
}

template <unsigned int N>
void StructuredGridEntity<N>::initTransformer(){
    SpatialEntity<N>::transformer_ = new StructuredCoordinateTransformer<N>(this);
}

template <unsigned int N>
Vector<N, unsigned int> StructuredGridEntity<N>::getDimension() const {
    return dimension_;
}
template <unsigned int N>
void StructuredGridEntity<N>::setDimension(const Vector<N, unsigned int>& dimension) {
    dimension_ = dimension;
}

/*---------------------------------------------------------------*/
/*--SpatialCoordinateTransformer--------------------------------- */
/*---------------------------------------------------------------*/

template<unsigned int N>
const Matrix<N+1, float> SpatialCoordinateTransformer<N>::getTextureToIndexMatrix() const {
    return getDimensionMatrix();
}
template<unsigned int N>
const Matrix<N+1, float> SpatialCoordinateTransformer<N>::getTextureToModelMatrix() const {
    return getBasisMatrix();
}
template<unsigned int N>
const Matrix<N+1, float> SpatialCoordinateTransformer<N>::getTextureToWorldMatrix() const {
    return getBasisMatrix()*getWorldMatrix();
}
template<unsigned int N>
const Matrix<N+1, float> SpatialCoordinateTransformer<N>::getIndexToTextureMatrix() const {
    return glm::inverse(getDimensionMatrix());
}
template<unsigned int N>
const Matrix<N+1, float> SpatialCoordinateTransformer<N>::getIndexToModelMatrix() const {
    return glm::inverse(getDimensionMatrix())*getBasisMatrix();
}
template<unsigned int N>
const Matrix<N+1, float> SpatialCoordinateTransformer<N>::getIndexToWorldMatrix() const {
    return glm::inverse(getDimensionMatrix())*getBasisMatrix()*getWorldMatrix();
}
template<unsigned int N>
const Matrix<N+1, float> SpatialCoordinateTransformer<N>::getModelToTextureMatrix() const {
    return glm::inverse(getBasisMatrix());
}
template<unsigned int N>
const Matrix<N+1, float> SpatialCoordinateTransformer<N>::getModelToIndexMatrix() const {
    return glm::inverse(getBasisMatrix())*getDimensionMatrix();
}
template<unsigned int N>
const Matrix<N+1, float> SpatialCoordinateTransformer<N>::getModelToWorldMatrix() const {
    return getWorldMatrix();
}
template<unsigned int N>
const Matrix<N+1, float> SpatialCoordinateTransformer<N>::getWorldToTextureMatrix() const {
    return glm::inverse(getWorldMatrix())*glm::inverse(getBasisMatrix());
}
template<unsigned int N>
const Matrix<N+1, float> SpatialCoordinateTransformer<N>::getWorldToIndexMatrix() const {
    return glm::inverse(getWorldMatrix())*glm::inverse(getBasisMatrix())*getDimensionMatrix();
}
template<unsigned int N>
const Matrix<N+1, float> SpatialCoordinateTransformer<N>::getWorldToModelMatrix() const {
    return glm::inverse(getWorldMatrix());
}


} // namespace
#endif //IVW_SPATIALDATA_H