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

#include <inviwo/core/datastructures/transferfunctiondatapoint.h>

namespace inviwo{
    TransferFunctionDataPoint::TransferFunctionDataPoint(){}
    TransferFunctionDataPoint::TransferFunctionDataPoint(vec2* pos):pos_(*pos){}
	TransferFunctionDataPoint::TransferFunctionDataPoint(vec2* pos, vec4* rgba):pos_(*pos), rgba_(*rgba){}
	TransferFunctionDataPoint::TransferFunctionDataPoint(vec2 pos, vec4 rgba):pos_(pos), rgba_(rgba){}
	TransferFunctionDataPoint::~TransferFunctionDataPoint() {}

    const vec2* TransferFunctionDataPoint::getPos(){
        return &pos_;
    }

	void TransferFunctionDataPoint::setPos(vec2 pos){
		pos_ = pos;
	}
	
	void TransferFunctionDataPoint::setPos(vec2* pos){
		pos_ = *pos;
	}

	void TransferFunctionDataPoint::setPos( float x, float y ){
		pos_ = vec2(x, y);
	}

    const vec4* TransferFunctionDataPoint::getRgba(){
        return &rgba_;
    }

	void TransferFunctionDataPoint::setRgba(vec4 rgba ){
		rgba_ = rgba;
	}
	
	void TransferFunctionDataPoint::setRgba(vec4* rgba ){
		rgba_ = *rgba;
	}

    void TransferFunctionDataPoint::setRgb(const vec3* rgb){
        rgba_.r = rgb->r;
        rgba_.g = rgb->g;
        rgba_.b = rgb->b;
    }

    void TransferFunctionDataPoint::setA(const float alpha ){
        rgba_.a = alpha;
    }

	const bool TransferFunctionDataPoint::isSelected(){
		return selected_;
	}

	void TransferFunctionDataPoint::setSelected( bool selected ){
		selected_ = selected;
        //if (selected)
        //    emit pointSelected(this);
	}
};