'''
/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.9
 *
 * Copyright (c) 2013-2015 Inviwo Foundation
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
 *********************************************************************************/
'''

import pypackagesutil #inviwo module that retrieves buffer data
import numpy as np #numpy

import pycuda #pycuda
import pycuda.driver as drv
import pycuda.tools
import pycuda.autoinit
from pycuda.compiler import SourceModule

#string buffer that holds CUDA kernel
CUDA_KERNEL_SRC = pypackagesutil.getBufferData("PyCUDAImageInverter", "CUDAKernelSrc")
mod = SourceModule(CUDA_KERNEL_SRC.tostring())

#input image (SourceImage) which needs to be inverted
SourceImage = pypackagesutil.getLayerData("PyCUDAImageInverter", "SourceImage")

# save Alpha Channel
SourceImageAlphaChannel = SourceImage[:,:,3]

#function to determine block and grid size
def determineBlockGridSize(cols, rows):
    bdim = (16, 16, 1) #use fixed block size for now
    dx, mx = divmod(cols, bdim[0])
    dy, my = divmod(rows, bdim[1])
    gdim = ( (dx + (mx>0)) * bdim[0], (dy + (my>0)) * bdim[1] )
    return bdim, gdim

#invoke CUDA kernel for image invert
invertKernel = mod.get_function("CUDAImageInvertKernel")

#compute block, grid dimension
imgWidth = SourceImage.shape[1]
imgHeight = SourceImage.shape[0]
b,g = determineBlockGridSize(cols=imgWidth, rows=imgHeight)

#launch kernel
invertKernel( drv.InOut(SourceImage), np.int32(imgWidth), np.int32(imgHeight), block=b, grid=g)

