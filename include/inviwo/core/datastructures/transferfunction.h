#ifndef IVW_TRANSFERFUNCTION_H
#define IVW_TRANSFERFUNCTION_H
#include <stdlib.h>
#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/datastructures/data.h>
#include <inviwo/core/datastructures/imagedisk.h>
#include <inviwo/core/datastructures/imageram.h>
#include <inviwo/core/ports/imageport.h>

namespace inviwo {

    class IVW_CORE_API TransferFunction : public Data {

    public:
        TransferFunction();
        virtual ~TransferFunction();
        virtual Data* clone();
        void setAlpha(ImageRAMfloat16*);
        ImageRAMfloat16* getAlpha();

    private:
        ImageRAMfloat16* alphaImage;
    };

} // namespace
#endif // IVW_TRANSFERFUNCTION_H
