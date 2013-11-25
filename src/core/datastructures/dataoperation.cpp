#include <inviwo/core/datastructures/dataoperation.h>

namespace inviwo {

DataOperation::DataOperation(){}

DataOperation::~DataOperation(){}

void DataOperation::setOutput(DataRepresentation* out) { 
    out_ = out; 
}

} // namespace
