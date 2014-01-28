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
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

uniform float minDepth_;
uniform float maxDepth_;

uniform float zNear_;
uniform float zFar_;

float normalizeDepth(float depth) {
    return depth;
    float result = depth - minDepth_;
    result /= (maxDepth_ - minDepth_);
    return result;
}

float normDepth(float depth) {
    // deprecated
    return normalizeDepth(depth);
}

float convertScreenToEye(float depthScreen) {
	float depthNDC = 2.0 * depthScreen - 1.0;
    float depthEye = 2.0 * zNear_ * zFar_ / (zFar_ + zNear_ - depthNDC * (zFar_ - zNear_));
	return depthEye;
}

float convertEyeToScreen(float depthEye) {
	float A = -(zFar_+zNear_)/(zFar_-zNear_);
    float B = (-2.0*zFar_*zNear_) / (zFar_-zNear_);
    float depthScreen = 0.5*(-A*depthEye + B) / depthEye + 0.5;
	return depthScreen;
}

float calculateDepthValue(float t, float entryDepthScreen, float exitDepthScreen) {
	// to calculate the correct depth values, which are not linear in the deph buffer,
	// we must first convert our screen space coordinates into eye coordinates and interpolate there.
	
	// transform into eye space
	float entryDepthEye = convertScreenToEye(entryDepthScreen);
	float exitDepthEye  = convertScreenToEye(exitDepthScreen);
	
	// compute the depth value in clip space
	float resultEye = entryDepthEye + t * (exitDepthEye - entryDepthEye);
	
	// transform back to screen space
	float resultScreen = convertEyeToScreen(resultEye);
	return resultScreen;
}