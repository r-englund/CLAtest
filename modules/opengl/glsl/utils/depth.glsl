/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2014 Inviwo Foundation
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

float convertScreenToEye(CAMERA_PARAMETERS camera, float depthScreen) {
    float depthNDC = 2.0 * depthScreen - 1.0;
    float depthEye = 2.0 * camera.zNear_ * camera.zFar_ /
                     (camera.zFar_ + camera.zNear_ - depthNDC * (camera.zFar_ - camera.zNear_));
    return depthEye;
}

float convertEyeToScreen(CAMERA_PARAMETERS camera, float depthEye) {
    float A = -(camera.zFar_+camera.zNear_)/(camera.zFar_-camera.zNear_);
    float B = (-2.0*camera.zFar_*camera.zNear_) / (camera.zFar_-camera.zNear_);
    float depthScreen = 0.5*(-A*depthEye + B) / depthEye + 0.5;
    return depthScreen;
}

float calculateDepthValue(CAMERA_PARAMETERS camera, float t, float entryDepthScreen, float exitDepthScreen) {
    // to calculate the correct depth values, which are not linear in the deph buffer,
    // we must first convert our screen space coordinates into eye coordinates and interpolate there.
    // transform into eye space
    float entryDepthEye = convertScreenToEye(camera, entryDepthScreen);
    float exitDepthEye  = convertScreenToEye(camera, exitDepthScreen);
    // compute the depth value in clip space
    float resultEye = entryDepthEye + t * (exitDepthEye - entryDepthEye);
    // transform back to screen space
    float resultScreen = convertEyeToScreen(camera, resultEye);
    return resultScreen;
}