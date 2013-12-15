/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Viktor Axelsson
 *
 **********************************************************************/

/** \Class for holding transfer function data
*
*  This class holds transfer function data, currently one parameter in the variable data_.
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
	int getTextureSize();

    float getMaskMin();
    float getMaskMax();
    void setMaskMin(float maskMin);
    void setMaskMax(float maskMax);

private:
    float maskMin_;
    float maskMax_;
    int textureSize_;
    Image* data_;
    std::vector<TransferFunctionDataPoint*> dataPoints_;
};

} // namespace
#endif // IVW_TRANSFERFUNCTION_H
