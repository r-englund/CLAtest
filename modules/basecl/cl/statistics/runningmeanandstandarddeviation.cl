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
 * Main file author: Daniel Jönsson
 *
 *********************************************************************************/

#include "samplers.cl" 
/*
 * Compute running average and standard deviation given new samples
 * http://en.wikipedia.org/wiki/Standard_deviation (Rapid calculation methods)
 */
__kernel void runningMeanAndStandardDeviationKernel(
      int2 nSamples
    , read_only image2d_t sampleImg
    , float iteration // 1 ... N
    , read_only image2d_t prevMeanImg
    , write_only image2d_t nextMeanImg
    , read_only image2d_t prevStdImg
    , write_only image2d_t newStdImg
    ) 
{ 
    //output image pixel coordinates 
    int2 globalId = (int2)(get_global_id(0), get_global_id(1));  
 
    int threadId = get_global_id(0) + get_global_id(1)*nSamples.x;
    if (any(globalId>=nSamples)) {
        return;
    }

    
    float4 sample = read_imagef(sampleImg, smpUNormNoClampNearest, globalId);  
    if (iteration != 1.f) {
        float4 prevMean = read_imagef(prevMeanImg, smpUNormNoClampNearest, globalId);  
        float4 prevStd = read_imagef(prevStdImg, smpUNormNoClampNearest, globalId);  
        float4 newMean = prevMean + (sample-prevMean)/iteration;
        float multiplier = 1.f;

        float4 Qprev = (iteration-2.f)*prevStd/multiplier;
        //float4 diff = (sample-prevMean);
        //float4 Q = Qprev + diff*diff*(iteration-1.f)/iteration;
        float4 Q = Qprev + (sample-prevMean)*(sample-newMean);
        float4 newStd = multiplier*Q/(iteration-1.f);
        newStd.w = 1.f;
        write_imagef(newStdImg, globalId, newStd);
        write_imagef(nextMeanImg, globalId, newMean);
        
    } else {
        write_imagef(newStdImg, globalId, (float4)(0.f));
        write_imagef(nextMeanImg, globalId, sample);
    }

    //

}