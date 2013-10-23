#ifndef IVW_COORDINATETRANSFORMER_H
#define IVW_COORDINATETRANSFORMER_H
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/data.h>

namespace inviwo {

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
    virtual const Matrix<N+1,float> getWorldMatrix() const {
        return spatialData_->getWorldTransform();
    }

    virtual const Matrix<N+1,float> getBasisMatrix() const {
        return spatialData_->getBasisAndOffset();
    }

    virtual const Matrix<N+1,float> getDimensionMatrix() const {
        Matrix<N+1,float> mat(1.0f);
        return mat;
    }

private:
    const SpatialData<N>* spatialData_;
};

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


template<unsigned int N>
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


} // namespace
#endif //IVW_COORDINATETRANSFORMER_H	