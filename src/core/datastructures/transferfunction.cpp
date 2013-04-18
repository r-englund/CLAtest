#include <inviwo/core/datastructures/transferfunction.h>

namespace inviwo {

    TransferFunction::TransferFunction(){
		data_.addRepresentation(new ImageRAMfloat32(uvec2(256,1)));
    }

    TransferFunction::~TransferFunction() {
    }

    void TransferFunction::setData(Image data){
        data_ = data;
    }
    
    Image* TransferFunction::getData() const{
        return const_cast<Image*>(&data_);
    }
}