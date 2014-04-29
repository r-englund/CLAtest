
#Remove hard coding
import pypackagesutil
import numpy as np
print np.__version__
#import ctypes

CUDA_KERNEL_SRC_PTR = pypackagesutil.getBufferData("PyCUDAImageInverter", "ImageInvert_KernelSrc")

#ArrayType = ctypes.c_uint8*94
#array_pointer = ctypes.cast(CUDA_KERNEL_SRC_PTR, ctypes.POINTER(ArrayType))
#a = np.frombuffer(array_pointer.contents)
#a = np.frombuffer(CUDA_KERNEL_SRC_PTR, np.uint8)
#print CUDA_KERNEL_SRC_PTR.tostring()
#print CUDA_KERNEL_SRC_PTR.__array_interface__
#print type(CUDA_KERNEL_SRC_PTR)
#CUDA_KERNEL_SRC_PTR.data[1:5] = 5
#np.put(CUDA_KERNEL_SRC_PTR, 0, 1)
#print CUDA_KERNEL_SRC_PTR.tostring()
#CUDA_KERNEL_SRC_PTR[0] = 1
#ptr = CUDA_KERNEL_SRC_PTR.__array_interface__['data'][0]
#print ptr
#print CUDA_KERNEL_SRC_PTR.tostring().lower()
for ind, c in enumerate(CUDA_KERNEL_SRC_PTR.tostring()):
    if c.islower():
         CUDA_KERNEL_SRC_PTR.data[ind]=c.upper()
    else:
    	CUDA_KERNEL_SRC_PTR.data[ind]=c.lower()
print CUDA_KERNEL_SRC_PTR.data