/** \Class for holding transfer function data
*
*  This class holds transfer function data, currently one parameter in the variable data_.
*  TODO: refactor to also hold control points instead of keeping them in widget
*/

#ifndef IVW_TRANSFERFUNCTION_H
#define IVW_TRANSFERFUNCTION_H
#include <stdlib.h>
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/image/imagedisk.h>
#include <inviwo/core/datastructures/image/imageram.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/datastructures/transferfunctiondatapoint.h>

namespace inviwo {

    class IVW_CORE_API TransferFunction {
    public:
        TransferFunction();
        virtual ~TransferFunction();
        void setData(Image);
        Image* getData() const;
        int getSize();
        TransferFunctionDataPoint* getPoint(int i);

		void addPoint(vec2 pos, vec4 rgba);
        void addPoint(vec2* pos, vec4* rgba);
        void addPoint(TransferFunctionDataPoint* newPoint);
        void removePoint(TransferFunctionDataPoint* newPoint);
        
        void sortDataPoints();
        void calcTransferValues();

    private:
        Image* data_;
        std::vector<TransferFunctionDataPoint*> dataPoints_;
    };

} // namespace
#endif // IVW_TRANSFERFUNCTION_H
