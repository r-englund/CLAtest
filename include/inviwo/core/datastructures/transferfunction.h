#ifndef IVW_TRANSFERFUNCTION_H
#define IVW_TRANSFERFUNCTION_H
#include <stdlib.h>
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/imagedisk.h>
#include <inviwo/core/datastructures/imageram.h>
#include <inviwo/core/ports/imageport.h>

namespace inviwo {

    class IVW_CORE_API TransferFunction {

    public:
        TransferFunction();
        virtual ~TransferFunction();
        //virtual Data* clone();
        void setData(Image);
        Image* getData() const;

    private:
		Image data_;
        //ImageRAMfloat32* alphaImage;
    };

} // namespace
#endif // IVW_TRANSFERFUNCTION_H
