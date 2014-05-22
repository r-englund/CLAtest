#include <inviwo/core/datastructures/datamapper.h>

namespace inviwo {

DataMapper::DataMapper(const DataFormatBase* format){
    initWithFormat(format);
}

DataMapper::DataMapper() {
    initWithFormat(DataUINT8::get());
}

void DataMapper::initWithFormat(const DataFormatBase* format){
    dataRange.y = format->getMax();
    switch (format->getNumericType()) {
        case DataFormatEnums::FLOAT_TYPE:
            dataRange.x = -format->getMax();
            break;
        case DataFormatEnums::UNSIGNED_INTEGER_TYPE:
            dataRange.x = 0.0;
            break;
        case DataFormatEnums::SIGNED_INTEGER_TYPE:
            dataRange.x = format->getMin();
            break;
        case DataFormatEnums::NOT_SPECIALIZED_TYPE:
            dataRange.x = format->getMin();
            break;
    }
    
    valueRange = dataRange;
    valueUnit = "arb. unit.";
}

} // namespace

