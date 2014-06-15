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

processor  = "SimpleRaycaster"
canvas     = "CanvasGL"
path = inviwo.getBasePath() + "/data/images/"
dvrType = "raycast"


dataPath = 'D:/Users/Daniel\Documents/My Box Files/Verifiable volume rendering/grid_convergence/'
# --- configuration ---

InviwoVersion = "0.6b"
def vvrRaySample(InviwoVersion = '0.6b',  windowRes = (512,512), dataPath = 'F:\src\data\grid_convergence', dataSet = 'grid_01.nhdr', maxDataDim = 2.0, samplesPerRay = 4.0, sampleIncement = 1.0, numSamples = 1):
    inviwo.setPropertyValue("VolumeSource", "filename", dataPath + dataSet)
    inviwo.setPropertyValue("VolumeSource", "dataRange", (0, 2))
    # start loop 
    samplesTaken = 0
    while (samplesTaken < numSamples):
            sampleRate = samplesPerRay/maxDataDim
            # update 
            inviwo.setPropertyValue(processor, "samplingRate", sampleRate)

            inviwo.snapshotCanvas(0, "%sversion(%s)_windowRes(%d,%d)_dvrType(%s)_dataset(%s)_samplesPerRay(%f).tif" % (path, InviwoVersion, windowRes[0], windowRes[1], dvrType, dataSet, samplesPerRay))

            samplesPerRay *= sampleIncement
            samplesTaken += 1


# resize canvas and initialize camera
inviwo.setPropertyValue(canvas, "dimensions", windowRes)

test = "grid_convergence"
#test = "ray_convergence"
if test == "grid_convergence":
    samplesPerRay = 100
    numSamples = 1.0
    dataSets = ['grid-01.dat', 'grid-02.dat', 'grid-03.dat', 'grid-04.dat', 'grid-05.dat', 'grid-06.dat', 'grid-07.dat', 'grid-08.dat', 'grid-09.dat', 'grid-10.dat']
    maxDataDim = (2.0, 3.0, 5.0, 9.0, 17.0, 33.0, 65.0, 129.0, 257.0, 513.0)
    #dataSets = ['grid-01.dat']
    #maxDataDim = [2.0]
    sampleIncrementPerRay = 1.0
else:
    samplesPerRay = 2
    numSamples = 9.0
    dataSets = ['grid-2x2x2.nhdr', 'grid-11x11x11.nhdr']
    maxDataDim = [2.0, 11.0]
    sampleIncrementPerRay = 2.0
    




dataDimCount = 0
for dataSet in dataSets:
    dataDim = maxDataDim[dataDimCount]
    #sampleRate = samplesPerRay/maxDataDim[dataDimCount]
    #sampleIncrement = 1.0 #sampleIncrementPerRay/dataMaxSizes[dataDimCount]
    vvrRaySample(InviwoVersion, windowRes, dataPath, dataSet, dataDim, samplesPerRay, sampleIncrementPerRay, numSamples)
    dataDimCount +=1

