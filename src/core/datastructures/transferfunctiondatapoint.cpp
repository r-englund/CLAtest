#include <inviwo/core/datastructures/transferfunctiondatapoint.h>

namespace inviwo{
    TransferFunctionDataPoint::TransferFunctionDataPoint(){}
    TransferFunctionDataPoint::TransferFunctionDataPoint(vec2* pos):pos_(*pos){}
	TransferFunctionDataPoint::TransferFunctionDataPoint(vec2* pos, vec4* rgba):pos_(*pos), rgba_(*rgba){}
	TransferFunctionDataPoint::TransferFunctionDataPoint(vec2 pos, vec4 rgba):pos_(pos), rgba_(rgba){}
	TransferFunctionDataPoint::~TransferFunctionDataPoint() {
		LogInfo("Datapoint destructor");
	}

    const vec2* TransferFunctionDataPoint::getPos(){
        return &pos_;
    }

	void TransferFunctionDataPoint::setPos(vec2 pos){
		pos_ = pos;
	}
	
	void TransferFunctionDataPoint::setPos(vec2* pos){
		pos_ = *pos;
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
	}

};