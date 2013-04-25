/** \Class for holding transfer function data
*
*  This class holds transfer function data, currently one parameter in the variable data_.
*  TODO: refactor to also hold control points instead of keeping them in widget
*/

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
        void setData(Image);
        Image* getData() const;

        vec2* getPosition(int);
        void setPosition(int, vec2*);

        vec4* getValue(int);
        void setValue(int, vec4*);

        void calcTransferValues();
        void createPoint(vec2* pos, vec4* val);

    private:
        Image data_;
        std::vector<vec2*> pointPositions_;
        std::vector<vec4*> pointValues_;
        vec4* dataArray_;
    };

} // namespace
#endif // IVW_TRANSFERFUNCTION_H
