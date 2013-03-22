#include <inviwo/core/datastructures/transferfunc.h>
#include <stdlib.h>
namespace inviwo {

    TransferFunc::TransferFunc() : Data() {
        alphaValues = new float[256];
        std::fill(alphaValues, alphaValues + 256, 0.5f);
    }
    TransferFunc::~TransferFunc() {
    }

    Data* TransferFunc::clone() {
        TransferFunc* newImage = new TransferFunc();
        copyRepresentations(newImage);
        return newImage;
    }

    void TransferFunc::setAlpha(float* alphaValues_){
        alphaValues = alphaValues_;
    }
    
    float* TransferFunc::getAlpha(){
        return alphaValues;
    }

}
