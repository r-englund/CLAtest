/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Contact: Timo Ropinski
 *
 *********************************************************************************/

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