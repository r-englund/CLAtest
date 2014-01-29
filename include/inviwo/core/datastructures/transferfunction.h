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
#include <inviwo/core/datastructures/image/layer.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/datastructures/transferfunctiondatapoint.h>

namespace inviwo {

class IVW_CORE_API TransferFunction {
public:

    enum InterpolationType {
        InterpolationLinear = 0,
        InterpolationCubic
    };

	TransferFunction();
	TransferFunction(const TransferFunction& rhs);
	TransferFunction& operator=(const TransferFunction& rhs);
	virtual ~TransferFunction();

    const Layer* getData() const { return data_; }
    size_t getNumDataPoints() const { return dataPoints_.size(); }
    int getTextureSize() { return textureSize_; }

	TransferFunctionDataPoint* getPoint(int i) const;

	void addPoint(const vec2& pos, const vec4& color);
	void addPoint(TransferFunctionDataPoint* dataPoint);
	void removePoint(TransferFunctionDataPoint* dataPoint);
    void clearPoints();
    void sortPoints();

	void calcTransferValues();

    float getMaskMin() { return maskMin_; }
    void setMaskMin(float maskMin) { maskMin_ = maskMin; }
    float getMaskMax() { return maskMax_; }
    void setMaskMax(float maskMax) { maskMax_ = maskMax; }

    void setInterpolationType(InterpolationType interpolationType) { interpolationType_ = interpolationType; }
    InterpolationType getInterpolationType() const { return interpolationType_; }

private:
    float maskMin_;
    float maskMax_;
    int textureSize_;
    Layer* data_;
    std::vector<TransferFunctionDataPoint*> dataPoints_;
    InterpolationType interpolationType_;
};

} // namespace
#endif // IVW_TRANSFERFUNCTION_H
