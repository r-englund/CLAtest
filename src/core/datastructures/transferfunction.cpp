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

#include <inviwo/core/datastructures/transferfunction.h>
#include <inviwo/core/datastructures/image/layerram.h>
#include <math.h>

namespace inviwo {

TransferFunction::TransferFunction()
    : VoidObservable() {
	textureSize_ = 1024;
    maskMin_ = 0.0f;
    maskMax_ = 1.0f;
    interpolationType_ = InterpolationLinear;
    data_ = new Layer(uvec2(textureSize_, 1), DataVec4FLOAT32::get());

    // initialize with standard ramp
    addPoint(new TransferFunctionDataPoint(this, vec2(0.0f,0.0f), vec4(0.0f,0.0f,0.0f,0.0f)));
    addPoint(new TransferFunctionDataPoint(this, vec2(1.0f,1.0f), vec4(1.0f,1.0f,1.0f,1.0f)));
}

TransferFunction::TransferFunction(const TransferFunction& rhs) {
	data_ = NULL;
	*this = rhs;
}

TransferFunction& TransferFunction::operator=(const TransferFunction& rhs) {
	if (this != &rhs) {
		delete data_;
		data_ = new Layer(*rhs.data_);
		clearPoints();
        textureSize_ = rhs.textureSize_;
        maskMin_ = rhs.maskMin_;
        maskMax_ = rhs.maskMax_;
        interpolationType_ = rhs.interpolationType_;
		for (size_t i=0; i<rhs.getNumDataPoints(); i++)
            addPoint(new TransferFunctionDataPoint(*rhs.getPoint(static_cast<int>(i))));
	}
	calcTransferValues();
	return *this;
}

TransferFunction::~TransferFunction(){
	clearPoints();
	delete data_;
}

TransferFunctionDataPoint* TransferFunction::getPoint(int i) const {
    ivwAssert(i<int(getNumDataPoints()), "Trying to access non-existent data point.");
	return dataPoints_[i];
}

void TransferFunction::addPoint(const vec2& pos, const vec4& color) {
	addPoint(new TransferFunctionDataPoint(this, pos, color));
}

void TransferFunction::addPoint(TransferFunctionDataPoint* dataPoint) {
	std::vector<TransferFunctionDataPoint*>::iterator iter = dataPoints_.begin();

	float pointPosition = dataPoint->getPos().x;
	float iterPosition;

	if (dataPoints_.size() == 0) {
		dataPoints_.push_back(dataPoint);
	}
	else{
		if (pointPosition > dataPoints_.back()->getPos().x) {
			dataPoints_.push_back(dataPoint);
		}
		else{
			for (iter = dataPoints_.begin(); iter != dataPoints_.end(); iter++) {
				iterPosition = (*iter)->getPos().x;
				if (iterPosition > pointPosition){
					dataPoints_.insert(iter, dataPoint);
					break;
				}
			}
		}
	}
	calcTransferValues();
}

void TransferFunction::removePoint(TransferFunctionDataPoint* dataPoint) {
    std::vector<TransferFunctionDataPoint*>::iterator it = std::find(dataPoints_.begin(), dataPoints_.end(), dataPoint);
    if (it != dataPoints_.end()) {
        delete *it;
        dataPoints_.erase(it);
    }
	calcTransferValues();
}

void TransferFunction::clearPoints() {
	if (dataPoints_.size() > 0){
		for (std::vector<TransferFunctionDataPoint*>::iterator iter = dataPoints_.begin(); iter != dataPoints_.end();){
			delete *iter;
			iter = dataPoints_.erase(iter);
		}
		dataPoints_.clear();
	}
}

void TransferFunction::calcTransferValues(){
    vec4* dataArray = static_cast<vec4*>(data_->getEditableRepresentation<LayerRAM>()->getData());

    // in case of 0 points
    if ((int)dataPoints_.size() == 0){
        for (int i = 0; i <= (textureSize_ - 1); i++){
            dataArray[i] = vec4((float)i/(textureSize_ - 1.0), (float)i/(textureSize_ - 1.0), (float)i/(textureSize_ - 1.0), 1.0);
        }
    }
    // in case of 1 point
    else if ((int)dataPoints_.size () == 1){ 
        for (size_t i = 0; i <= (size_t)(textureSize_ - 1) ; ++i){
            dataArray[i] = dataPoints_[0]->getRGBA();
        }
    }

    // in case of more than 1 points
    else {
        int leftX = (int)ceil(dataPoints_.front()->getPos().x*(textureSize_-1));
        int rightX = (int)ceil(dataPoints_.back()->getPos().x*(textureSize_-1));

        for (int i=0; i<=leftX; i++)
            dataArray[i] = dataPoints_.front()->getRGBA();
        for (int i=rightX; i<textureSize_; i++)
            dataArray[i] = dataPoints_.back()->getRGBA();

        //if (interpolationType_==TransferFunction::InterpolationLinear) {
            // linear interpolation
            std::vector<TransferFunctionDataPoint*>::iterator pLeft = dataPoints_.begin();
            std::vector<TransferFunctionDataPoint*>::iterator pRight = dataPoints_.begin() + 1;
            while (pRight != dataPoints_.end()) {
                int n = (int)ceil((*pLeft)->getPos().x * (textureSize_-1));
                while (n < ceil((*pRight)->getPos().x * (textureSize_-1))) {
                    vec4 lrgba = (*pLeft)->getRGBA();
                    vec4 rrgba = (*pRight)->getRGBA();
                    float lx = (*pLeft)->getPos().x * (textureSize_-1);
                    float rx = (*pRight)->getPos().x * (textureSize_-1);
                    float alpha = (n - lx)/(rx - lx);
                    dataArray[n] = alpha*rrgba + (1.0f-alpha)*lrgba;
                    n++;
                }
                pLeft++;
                pRight++;
            }
        //} else {
            // cubic interpolation
            // TODO: implement cubic interpolation
        //}
    }

    for (int i=0; i<int(maskMin_*textureSize_); i++)
        dataArray[i].a = 0.0;
    for (int i=int(maskMax_*textureSize_); i<textureSize_; i++)
        dataArray[i].a = 0.0;
    
    notifyObservers();
}

}