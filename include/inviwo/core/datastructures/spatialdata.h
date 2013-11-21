#ifndef IVW_SPATIALDATA_H
#define IVW_SPATIALDATA_H
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/data.h>

namespace inviwo {

/** \brief A convenience class to generate transformation matrices between  
 *         the different coordinate systems in use.
 *
 *  Spatial data in Inviwo uses 4 different coordinate systems, they are defined as
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

template <unsigned int N> class SpatialData;

template<unsigned int N>
class SpatialCoordinateTransformer : public CoordinateTransformer<N> {
public:
    SpatialCoordinateTransformer(const SpatialData<N>* spatialData) : CoordinateTransformer<N>(), spatialData_(spatialData) {};
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
    const SpatialData<N>* spatialData_;
};

template <unsigned int N> class StructuredData;
template <unsigned int N>
class StructuredCoordinateTransformer : public SpatialCoordinateTransformer<N> {
public:
    StructuredCoordinateTransformer(const StructuredData<N>* structuredData) : SpatialCoordinateTransformer<N>(structuredData), structuredData_(structuredData) {};
    virtual ~StructuredCoordinateTransformer(){};

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
    virtual SpatialData<N>* clone() const = 0;

    Vector<N,float> getOffset() const;
    void setOffset(const Vector<N,float>& offset);

    // Using row vectors in basis
    Matrix<N,float> getBasis() const;
    void setBasis(const Matrix<N,float>& basis);

    Matrix<N+1,float> getBasisAndOffset() const;
    void setBasisAndOffset(const Matrix<N+1,float>& mat);

    Matrix<N+1,float> getWorldMatrix() const;
    void setWorldMatrix(const Matrix<N+1,float>& mat);

    virtual const CoordinateTransformer<N>& getCoordinateTransformer() const;

protected:
    virtual void initTransformer();
    CoordinateTransformer<N>* transformer_;

};

template <unsigned int N>
class StructuredData : public SpatialData<N> {
public:
    StructuredData(const Vector<N, unsigned int>& dimension);
    StructuredData(const Vector<N,float>& offset, 
        const Vector<N, unsigned int>& dimension);
    StructuredData(const Matrix<N,float>& basis, 
        const Vector<N, unsigned int>& dimension);
    StructuredData(const Matrix<N,float>& basis, 
        const Vector<N,float>& offset, 
        const Vector<N, unsigned int>& dimension);

    virtual ~StructuredData(){}
    virtual StructuredData<N>* clone() const = 0;

    Vector<N, unsigned int> getDimension() const;
    void setDimension(const Vector<N, unsigned int>& dimension);

protected:
    virtual void initTransformer();
};


/*---------------------------------------------------------------*/
/*--Implementations----------------------------------------------*/
/*--SpatialData--------------------------------------------------*/
/*---------------------------------------------------------------*/

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
    return spatialData_->getWorldMatrix();
}

template <unsigned int N>
Vector<N,float> SpatialData<N>::getOffset() const {
    Vector<N,float> offset(0.0f);
    Matrix<N+1,float> mat = getBasisAndOffset();
    for(int i=0;i<N;i++){
        offset[i] = mat[N][i];
    }
    return offset;
}
template <unsigned int N>
void SpatialData<N>::setOffset(const Vector<N,float>& offset) {
     Matrix<N+1,float> mat = getBasisAndOffset();
     for(int i=0;i<N;i++){
        mat[N][i] = offset[i];
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
        mat[N][i] = -1.0f;
    }
    mat[N][N]=1.0f;
    return Data::getMetaData<MatrixMetaData<N+1,float> >("basisAndOffset", mat);
}
template <unsigned int N>
void SpatialData<N>::setBasisAndOffset(const Matrix<N+1,float>& mat) {
    Data::setMetaData<MatrixMetaData<N+1,float> >("basisAndOffset", mat);
}

template <unsigned int N>
Matrix<N+1,float> SpatialData<N>::getWorldMatrix() const {
    Matrix<N+1,float> mat(1.0f);
    return Data::getMetaData<MatrixMetaData<N+1,float> >("worldTransform", mat);
}
template <unsigned int N>
void SpatialData<N>::setWorldMatrix(const Matrix<N+1,float>& mat) {
    Data::setMetaData<MatrixMetaData<N+1,float> >("worldTransform", mat);
}

template <unsigned int N>
const CoordinateTransformer<N>& SpatialData<N>::getCoordinateTransformer() const{
    return *transformer_;
}

/*---------------------------------------------------------------*/
/*--StructuredData-----------------------------------------------*/
/*---------------------------------------------------------------*/

template <unsigned int N>
StructuredData<N>::StructuredData(const Vector<N, unsigned int>& dimension)
    : SpatialData<N>() {
    setDimension(dimension);
}

template <unsigned int N>
StructuredData<N>::StructuredData(const Vector<N,float>& offset, 
                                  const Vector<N, unsigned int>& dimension) 
    : SpatialData<N>(offset) {
    setDimension(dimension);
}

template <unsigned int N>
StructuredData<N>::StructuredData(const Matrix<N,float>& basis, 
                                  const Vector<N, unsigned int>& dimension)
    : SpatialData<N>(basis) {
    setDimension(dimension);
}
template <unsigned int N>
StructuredData<N>::StructuredData(const Matrix<N,float>& basis, 
                                  const Vector<N,float>& offset, 
                                  const Vector<N, unsigned int>& dimension)
    : SpatialData<N>(basis, offset) {
    setDimension(dimension);
}

template <unsigned int N>
void StructuredData<N>::initTransformer(){
    SpatialData<N>::transformer_ = new StructuredCoordinateTransformer<N>(this);
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