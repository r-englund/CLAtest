#include <inviwo/core/datastructures/transferfunction.h>

namespace inviwo {

    TransferFunction::TransferFunction() : Data() {
        alphaImage = new ImageRAMfloat16(uvec2(1,256));
    }

    TransferFunction::~TransferFunction() {
    }

    Data* TransferFunction::clone() {
        TransferFunction* newImage = new TransferFunction();
        copyRepresentations(newImage);
        return newImage;
    }

    void TransferFunction::setAlpha(ImageRAMfloat16* alphaImage_){
        alphaImage = alphaImage_;
    }
    
    ImageRAMfloat16* TransferFunction::getAlpha(){
        return alphaImage;
    }
}