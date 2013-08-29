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
//template <typename T>
class IVW_CORE_API TransferFunction {
public:
	TransferFunction();

	TransferFunction(const TransferFunction& rhs);
	TransferFunction& operator=(const TransferFunction& rhs);
	virtual ~TransferFunction();
	void setData(Image);
	const Image* getData() const;
	size_t getNumberOfDataPoints() const;
	TransferFunctionDataPoint* getPoint(int i) const;

	void addPoint(vec2 pos, vec4 rgba);
	void addPoint(vec2* pos, vec4* rgba);
	void addPoint(TransferFunctionDataPoint* newPoint);
	void removePoint(TransferFunctionDataPoint* newPoint);
	void clearPoints();

	void sortDataPoints();
	void calcTransferValues();
	void setDataBits(int bits);
	int getTextureSize();

private:
int textureSize_;
Image* data_;
DataFormatBase dataFormat_;
std::vector<TransferFunctionDataPoint*> dataPoints_;
};

} // namespace
#endif // IVW_TRANSFERFUNCTION_H
