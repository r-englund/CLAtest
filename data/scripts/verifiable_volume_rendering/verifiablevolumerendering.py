#/*********************************************************************************
# *
# * Inviwo - Interactive Visualization Workshop
# * Version 0.6b
# *
# * Copyright (c) 2013-2014 Inviwo Foundation
# * All rights reserved.
# *
# * Redistribution and use in source and binary forms, with or without
# * modification, are permitted provided that the following conditions are met:
# *
# * 1. Redistributions of source code must retain the above copyright notice, this
# * list of conditions and the following disclaimer.
# * 2. Redistributions in binary form must reproduce the above copyright notice,
# * this list of conditions and the following disclaimer in the documentation
# * and/or other materials provided with the distribution.
# *
# * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
# * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# *
# * Main file author: Daniel J?nsson
# *
# *********************************************************************************/

# Script to generate images for convergence test of volume renderer
# based on article Verifying Volume Rendering Using Discretization Error Analysis
# http://scivis.itn.liu.se/publications/2014/EJRSCNKYS14/

import inviwo 
import inviwoqt 

# --- configuration ---
windowRes = (512, 512)

#processor  = "VolumeRaycasterCL"
processor  = "SimpleRaycaster"
canvas     = "CanvasGL"
path = inviwo.getBasePath() + "/data/images/vvr/"
dvrType = "raycast"


dataPath =  inviwo.getBasePath() + "data/volumes/verifyablevolumerendering/"

# Default values
startWindowRes = 512
endWindowRes = 512
imagePath = path
startSamplesPerRay = 1024
endSamplesPerRay = 1024
#dataSets = ['grid-2x2x2.dat']
#maxDataDim = [2.0]
dataSets = ['grid-01.dat']
maxDataDim = [2.0]
#test = "dataset_convergence"
#test = "stepsize_convergence"
test = "pixel_convergence"
if test == "dataset_convergence":
    dataSets = ['grid-01.dat', 'grid-02.dat', 'grid-03.dat', 'grid-04.dat', 'grid-05.dat', 'grid-06.dat', 'grid-07.dat', 'grid-08.dat', 'grid-09.dat']
    maxDataDim = (2.0, 3.0, 5.0, 9.0, 17.0, 33.0, 65.0, 129.0, 257.0)
elif test == "stepsize_convergence":
    startSamplesPerRay = 1.0
    endSamplesPerRay = 256
else:
    startWindowRes = 32
    endWindowRes = 1024


dataDimCount = 0
iteration = 0

for dataSet in dataSets:
    dataDim = maxDataDim[dataDimCount]
    inviwo.setPropertyValue("VolumeSource", "filename", dataPath + dataSet)
    inviwo.setPropertyValue("VolumeSource", "dataRange", (0, 1))
    
    windowRes = startWindowRes
    while windowRes <= endWindowRes:
        inviwo.setPropertyValue(canvas, "dimensions", (windowRes, windowRes))
        samples = startSamplesPerRay
        while (samples <= endSamplesPerRay):
            sampleRate = samples/dataDim
            print sampleRate
            inviwo.setPropertyValue(processor, "samplingRate", sampleRate)
            inviwo.snapshotCanvas(0, "%siteration_%d.png" % (path + test + "/", iteration))
            samples *= 2
            iteration += 1
        windowRes *= 2
    dataDimCount +=1

