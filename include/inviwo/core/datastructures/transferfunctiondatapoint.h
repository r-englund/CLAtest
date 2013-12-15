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

#ifndef IVW_TRANSFERFUNCTIONDATAPOINT_H
#define IVW_TRANSFERFUNCTIONDATAPOINT_H

#include <stdlib.h>
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/image/imagedisk.h>
#include <inviwo/core/datastructures/image/imageram.h>
#include <inviwo/core/ports/imageport.h>

namespace inviwo {

class IVW_CORE_API TransferFunctionDataPoint {

public:
	TransferFunctionDataPoint();
	TransferFunctionDataPoint(vec2* pos);
	TransferFunctionDataPoint(vec2 pos, vec4 rgba);
	TransferFunctionDataPoint(vec2* pos, vec4* rgba);
    virtual ~TransferFunctionDataPoint();

	const vec2* getPos();
	void setPos(vec2 pos);
	void setPos(vec2* pos);
	void setPos(float x, float y);
	const vec4* getRgba();
	void setRgba(vec4 rgba);
	void setRgba(vec4* rgba);
    void setRgb(const vec3* rgb);
    void setA(const float alpha);
	const bool isSelected();
	void setSelected(bool);

private:
	vec2 pos_;
    vec4 rgba_;
	bool selected_;
};

} // namespace
#endif // IVW_TRANSFERFUNCTIONDATAPOINT_H
