
#Remove hard coding
import pypackagesutil
import numpy as np
#import matplotlib.pyplot as plt

#TODO: Incomplete under testing
CUDA_KERNEL_SRC = pypackagesutil.getBufferData("PyCUDAImageInverter", "CUDAKernelSrc")
SourceImage = pypackagesutil.getLayerData("PyCUDAImageInverter", "SourceImage")

print CUDA_KERNEL_SRC.data
print len(SourceImage)
print SourceImage.dtype
print SourceImage.shape
print SourceImage.itemsize
print SourceImage.ndim
print SourceImage

#imgplot = plt.imshow(SourceImage)
#plt.show()
#plt.hold(False)

