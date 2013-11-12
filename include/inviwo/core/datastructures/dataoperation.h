#ifndef IVW_DATAOPERATION_H
#define IVW_DATAOPERATION_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {

class DataRepresentation;

class IVW_CORE_API DataOperation {
public:
    DataOperation(){}
    virtual ~DataOperation(){}

    void setOutput(DataRepresentation* out) { out_ = out; }

    template<typename T>
    T* getOutput() { return static_cast<T*>(out_); }

private:
    DataRepresentation* out_;
};

} // namespace

#endif // IVW_DATAOPERATION_H
