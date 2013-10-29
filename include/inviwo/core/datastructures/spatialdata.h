#ifndef IVW_SPATIALDATA_H
#define IVW_SPATIALDATA_H
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/data.h>

namespace inviwo {

/** \brief A convenience class to generate transformation matrices between  
 *         the different coordinate systems in use.
 *
 *  Spatial data in inviwo uses 4 different coordinate systems, they are defined as
 *  * Index - The actual voxel indices in the data 
 *  * Model - Defines a local basis and offset for the data.
 *  * World - Puts the data at a position and angle in the scene.
 *  * Texture - The corresponding texture coordinates of the data.
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

template <unsigned int N> class SpatialData;

template<unsigned int N>
class SpatialCoordinateTransformer : public CoordinateTransformer<N> {
public:
    SpatialCoordinateTransformer(const SpatialData<N>* spatialData) : CoordinateTransformer<N>(), spatialData_(spatialData) {};
    virtual ~SpatialCoordinateTransformer(){};
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

protected:
    virtual const Matrix<N+1,float> getWorldMatrix() const;
    virtual const Matrix<N+1,float> getBasisMatrix() const;
    virtual const Matrix<N+1,float> getDimensionMatrix() const;

private:
    const SpatialData<N>* spatialData_;
};

template <unsigned int N> class StructuredData;
template <unsigned int N>
class StructuredCoordinateTransformer : public SpatialCoordinateTransformer<N> {
public:
    StructuredCoordinateTransformer(const StructuredData<N>* structuredData) : SpatialCoordinateTransformer<N>(structuredData), structuredData_(structuredData) {};
    virtual ~StructuredCoordinateTransformer(){};

protected:
    virtual const Matrix<N+1,float> getDimensionMatrix(){
        Vector<N,unsigned int> dim = structuredData_->getDimension();
        Matrix<N+1,float> mat(0.0f);
        for(int i=0;i<N;i++)
            mat[i][i]=(float)dim[i];
        mat[N+1][N+1]=1.0f;
        return mat;
    }

private:
    const StructuredData<N>* structuredData_;
};

             
template <unsigned int N>
class SpatialData : public Data {
public:
    SpatialData();
    SpatialData(const SpatialData&);
    SpatialData(const Vector<N,float>& offset);
    SpatialData(const Matrix<N,float>& basis);
    SpatialData(const Matrix<N+1,float>& mat);
    SpatialData(const Matrix<N,float>& basis, const Vector<N,float>& offset);

    virtual ~SpatialData();
    virtual SpatialData<N>* clone() const;

    Vector<N,float> getOffset() const;
    void setOffset(const Vector<N,float>& offset);

    // Using column vectors in basis
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

};

template <unsigned int N>
class StructuredData : public SpatialData<N> {
public:
    StructuredData(const Vector<N, unsigned int>& dimension, 
        const DataFormatBase* format);
    StructuredData(const Vector<N,float>& offset, 
        const Vector<N, unsigned int>& dimension, 
        const DataFormatBase* format);
    StructuredData(const Matrix<N,float>& basis, 
        const Vector<N, unsigned int>& dimension, 
        const DataFormatBase* format);
    StructuredData(const Matrix<N,float>& basis, 
        const Vector<N,float>& offset, 
        const Vector<N, unsigned int>& dimension, 
        const DataFormatBase* format);

    virtual ~StructuredData(){}
    virtual StructuredData<N>* clone() const;

    Vector<N, unsigned int> getDimension() const;
    void setDimension(const Vector<N, unsigned int>& dimension);

protected:
    virtual void initTransformer();
};


template <unsigned int N>
SpatialData<N>::SpatialData() : Data(), transformer_(NULL) {
    Vector<N,float> offset(-1.0f);
    Matrix<N,float> basis(2.0f);
    setBasis(basis);
    setOffset(offset);
    initTransformer();
}

template <unsigned int N>
SpatialData<N>::SpatialData(const SpatialData& rhs) : Data(rhs), transformer_(NULL){
     initTransformer();
}

template <unsigned int N>
SpatialData<N>::SpatialData(const Vector<N,float>& offset) : Data(), transformer_(NULL) {
    Matrix<N,float> basis(2.0f);
    setBasis(basis);
    setOffset(offset);
    initTransformer();
}
template <unsigned int N>
SpatialData<N>::SpatialData(const Matrix<N,float>& basis) : Data(), transformer_(NULL) {
    Vector<N,float> offset(-1.0f);
    setBasis(basis);
    setOffset(offset);
    initTransformer();
}
template <unsigned int N>
SpatialData<N>::SpatialData(const Matrix<N+1,float>& mat) : Data(), transformer_(NULL) {
    setBasisAndOffset(mat);
    initTransformer();
}
template <unsigned int N>
SpatialData<N>::SpatialData(const Matrix<N,float>& basis, const Vector<N,float>& offset) : Data(), transformer_(NULL) {
    setBasis(basis);
    setOffset(offset);
    initTransformer();
}

template <unsigned int N>
void SpatialData<N>::initTransformer(){
    transformer_ = new SpatialCoordinateTransformer<N>(this);
}

template <unsigned int N>
SpatialData<N>::~SpatialData(){
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
SpatialData<N>* SpatialData<N>::clone() const {
    return new SpatialData<N>(*this);
}

template <unsigned int N>
Vector<N,float> SpatialData<N>::getOffset() const {
    Vector<N,float> offset(0.0f);
    Matrix<N+1,float> mat = getBasisAndOffset();
    for(int i=0;i<N;i++){
        offset[i] = mat[i][N];
    }
    return offset;
}
template <unsigned int N>
void SpatialData<N>::setOffset(const Vector<N,float>& offset) {
     Matrix<N+1,float> mat = getBasisAndOffset();
     for(int i=0;i<N;i++){
        mat[i][N] = offset[i];
     }
     setBasisAndOffset(mat);
}

template <unsigned int N>
Matrix<N,float> SpatialData<N>::getBasis() const {
    Matrix<N,float> basis(1.0f);
    Matrix<N+1,float> mat = getBasisAndOffset();
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            basis[i][j] = mat[i][j];
        }
    }
    return basis;
}

template <unsigned int N>
void SpatialData<N>::setBasis(const Matrix<N,float>& basis) {
    Matrix<N+1,float> mat = getBasisAndOffset();
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            mat[i][j] = basis[i][j];
        }
    }
    setBasisAndOffset(mat);
}

template <unsigned int N>
Matrix<N+1,float> SpatialData<N>::getBasisAndOffset() const {
    Matrix<N+1,float> mat(2.0f);
    for(int i=0;i<N;i++){
        mat[i][N] = -1.0f;
    }
    mat[N][N]=1.0f;
    return Data::getMetaData<MatrixMetaData<N+1,float> >("basisAndOffset", mat);
}
template <unsigned int N>
void SpatialData<N>::setBasisAndOffset(const Matrix<N+1,float>& mat) {
    Data::setMetaData<MatrixMetaData<N+1,float> >("basisAndOffset", mat);
}

template <unsigned int N>
Matrix<N+1,float> SpatialData<N>::getWorldTransform() const {
    Matrix<N+1,float> mat(1.0f);
    return Data::getMetaData<MatrixMetaData<N+1,float> >("worldTransform", mat);
}
template <unsigned int N>
void SpatialData<N>::setWorldTransform(const Matrix<N+1,float>& mat) {
    Data::setMetaData<MatrixMetaData<N+1,float> >("worldTransform", mat);
}

template <unsigned int N>
const CoordinateTransformer<N>& SpatialData<N>::getCoordinateTransformer() const{
    return *transformer_;
}

/*---------------------------------------------------------------*/

template <unsigned int N>
StructuredData<N>::StructuredData(const Vector<N, unsigned int>& dimension, 
                                  const DataFormatBase* format) :
    SpatialData<N>() {
    setDimension(dimension);
    Data::setDataFormat(format);
}

template <unsigned int N>
StructuredData<N>::StructuredData(const Vector<N,float>& offset, 
                                  const Vector<N, unsigned int>& 
                                  dimension, const DataFormatBase* format) : 
    SpatialData<N>(offset) {
    setDimension(dimension);
    Data::setDataFormat(format);
}

template <unsigned int N>
StructuredData<N>::StructuredData(const Matrix<N,float>& basis, 
                                  const Vector<N, unsigned int>& dimension, 
                                  const DataFormatBase* format) : 
    SpatialData<N>(basis) {
    setDimension(dimension);
    Data::setDataFormat(format);
}
template <unsigned int N>
StructuredData<N>::StructuredData(const Matrix<N,float>& basis, 
                                  const Vector<N,float>& offset, 
                                  const Vector<N, unsigned int>& dimension,
                                  const DataFormatBase* format) : 
    SpatialData<N>(basis, offset) {
    setDimension(dimension);
    Data::setDataFormat(format);
}

template <unsigned int N>
void StructuredData<N>::initTransformer(){
    SpatialData<N>::transformer_ = new StructuredCoordinateTransformer<N>(this);
}

template <unsigned int N>
StructuredData<N>* StructuredData<N>::clone() const {
    return new StructuredData<N>(*this);
}

template <unsigned int N>
Vector<N, unsigned int> StructuredData<N>::getDimension() const {
    Vector<N, unsigned int> dimension;
    return Data::getMetaData<VectorMetaData<N, unsigned int> >("dimension", dimension);
}
template <unsigned int N>
void StructuredData<N>::setDimension(const Vector<N, unsigned int>& dimension) {
    Data::setMetaData<VectorMetaData<N, unsigned int> >("dimension", dimension);
}



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