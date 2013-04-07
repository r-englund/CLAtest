#ifndef IVW_TRANSFERFUNC_H
#define IVW_TRANSFERFUNC_H
#include <stdlib.h>
#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/datastructures/data.h>

namespace inviwo {

    class IVW_CORE_API TransferFunction : public Data {

    public:
        TransferFunction();
        virtual ~TransferFunction();
        virtual Data* clone();
        void setAlpha(float*);
        float* getAlpha();

    private:
        float* alphaValues;
    };

} // namespace

#endif // IVW_TRANSFERFUNC_H
