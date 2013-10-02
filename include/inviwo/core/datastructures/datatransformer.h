#ifndef IVW_SPATIALDATATRANSFORMER_H
#define IVW_SPATIALDATATRANSFORMER_H
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/data.h>

namespace inviwo {

template<unsigned int N>
class SpatialDataTransformer {
public:
	SpatialDataTransformer(SpatialData<N>* spatialData) : spatialData_(spatialData) {};
	virtual ~SpatialDataTransformer(){};

	/**
	 * Returns the matrix transformation mapping from texture coordinates
	 * to voxel index coordinates, i.e. from [0,1] to [0, number of voxels-1)
	 */
	virtual Matrix<N, float> getTextureToIndexMatrix() const;

	/**
	 * Returns the matrix transformation mapping from texture coordinates
	 * to data model coordinates, i.e. from [0,1] to (data min, data max)
	 */
	virtual Matrix<N, float> getTextureToModelMatrix() const;

	/**
	 * Returns the matrix transformation mapping from texture coordinates
	 * to opengl world coordinates, i.e. from [0,1] to (-inf, inf)
	 */
	virtual Matrix<N, float> getTextureToWorldMatrix() const;

	/**
	 * Returns the matrix transformation mapping from voxel index coordinates
	 * to texture coordinates, i.e. from [0, number of voxels-1) to [0,1]
	 */
	virtual Matrix<N, float> getIndexToTextureMatrix() const;

	/**
	 * Returns the matrix transformation mapping from voxel index coordinates
	 * to data model coordinates, i.e. from [0, number of voxels-1) to (data min, data max)
	 */
	virtual Matrix<N, float> getIndexToModelMatrix() const;

	/**
	 * Returns the matrix transformation mapping from voxel index coordinates
	 * to opengl world coordinates, i.e. from [0, number of voxels-1) to (-inf, inf)
	 */
	virtual Matrix<N, float> getIndexToWorldMatrix() const;

	/**
	 * Returns the matrix transformation mapping from data model coordinates
	 * to texture coordinates, i.e. from (data min, data max) to [0,1]
	 */
	virtual Matrix<N, float> getModelToTextureMatrix() const;

	/**
	 * Returns the matrix transformation mapping from data model coordinates
	 * to voxel index coordinates, i.e. from (data min, data max) to [0, number of voxels-1)
	 */
	virtual Matrix<N, float> getModelToIndexMatrix() const;

	/**
	 * Returns the matrix transformation mapping from data model coordinates
	 * to opengl world coordinates, i.e. from (data min, data max) to (-inf, inf)
	 */
	virtual Matrix<N, float> getModelToWorldMatrix() const;

	/**
	 * Returns the matrix transformation mapping from opengl world coordinates
	 * to texture coordinates, i.e. from (-inf, inf) to [0,1]
	 */
	virtual Matrix<N, float> getWorldToTextureMatrix() const;

	/**
	 * Returns the matrix transformation mapping from opengl world coordinates
	 * to voxel index coordinates, i.e. from (-inf, inf) to [0, number of voxels-1)
	 */
	virtual Matrix<N, float> getWorldToIndexMatrix() const;

	/**
	 * Returns the matrix transformation mapping from opengl world coordinates
	 * to data model coordinates, i.e. from (-inf, inf) to (data min, data max)
	 */
	virtual Matrix<N, float> getWorldToModelMatrix() const;



protected:
	virtual Matrix<N+1,float> getWorldMatrix(){
		return spatialData_->getWorldTransform();
	}

	virtual Matrix<N+1,float> getBasisMatrix(){
		return spatialData_->getBasisAndOffset();
	}

	virtual Matrix<N+1,float> getDimensionMatrix(){
		Matrix<N+1,float> mat(1.0f);
		return mat;
	}

private:
	SpatialData<N>* spatialData_;
};

template<unsigned int N>
Matrix<N, float> SpatialData<N>::getTextureToIndexMatrix() const {
	return getDimensionMatrix();
}
template<unsigned int N>
Matrix<N, float> SpatialData<N>::getTextureToModelMatrix() const {
	return getBasisMatrix();
}
template<unsigned int N>
Matrix<N, float> SpatialData<N>::getTextureToWorldMatrix() const {
	return getBasisMatrix()*getWorldMatrix();
}
template<unsigned int N>
Matrix<N, float> SpatialData<N>::getIndexToTextureMatrix() const {
	return glm::inverse(getDimensionMatrix());
}
template<unsigned int N>
Matrix<N, float> SpatialData<N>::getIndexToModelMatrix() const {
	return glm::inverse(getDimensionMatrix())*getBasisMatrix();
}
template<unsigned int N>
Matrix<N, float> SpatialData<N>::getIndexToWorldMatrix() const {
	return glm::inverse(getDimensionMatrix())*getBasisMatrix()*getWorldMatrix();
}
template<unsigned int N>
Matrix<N, float> SpatialData<N>::getModelToTextureMatrix() const {
	return glm::inverse(getBasisMatrix());
}
template<unsigned int N>
Matrix<N, float> SpatialData<N>::getModelToIndexMatrix() const {
	return glm::inverse(getBasisMatrix())*getDimensionMatrix();
}
template<unsigned int N>
Matrix<N, float> SpatialData<N>::getModelToWorldMatrix() const {
	return getWorldMatrix();
}
template<unsigned int N>
Matrix<N, float> SpatialData<N>::getWorldToTextureMatrix() const {
	return glm::inverse(getWorldMatrix())*glm::inverse(getBasisMatrix());
}
template<unsigned int N>
Matrix<N, float> SpatialData<N>::getWorldToIndexMatrix() const {
	return glm::inverse(getWorldMatrix())*glm::inverse(getBasisMatrix())*getDimensionMatrix();
}
template<unsigned int N>
Matrix<N, float> SpatialData<N>::getWorldToModelMatrix() const {
	return glm::inverse(getWorldMatrix());
}


template<unsigned int N>
class StructuredDataTransformer : public SpatialDataTransformer<N>(){
public:
	StructuredDataTransformer(StructuredData<N>* structuredData) : SpatialDataTransformer<N>(structuredData), structuredData_(structuredData) {};
	virtual ~StructuredDataTransformer(){};

protected:
	virtual Matrix<N+1,float> getDimensionMatrix(){
		Vector<N,unsigned int> dim = structuredData_->getDimension();
		Matrix<N+1,float> mat(0.0f);
		for(int i=0;i<N;i++)
			mat[i][i]=(float)dim[i];
		mat[N+1][N+1]=1.0f;
		return mat;
	}

private:
	StructuredData<N>* structuredData_;

};


} // namespace
#endif //IVW_SATIALDATATRANSFORMER_H	