#include <inviwo/core/datastructures/transferfunction.h>

namespace inviwo {

    TransferFunction::TransferFunction(){
        //alphaImage = new ImageRAMfloat32(uvec2(1,256));
		data_.addRepresentation(new ImageRAMfloat32(uvec2(256,1)));
    }

    TransferFunction::~TransferFunction() {
    }

    //Data* TransferFunction::clone() {
    //    TransferFunction* newImage = new TransferFunction();
    //    copyRepresentations(newImage);
    //    return newImage;
    //}

    void TransferFunction::setData(Image data){
        data_ = data;
    }
    
    Image* TransferFunction::getData() const{
        return const_cast<Image*>(&data_);
    }
}