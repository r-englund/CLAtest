
import pypackagesutil
import numpy as np

intArray = pypackagesutil.getBufferData("NumpyBufferTest", "integerBuffer")
uintArray = pypackagesutil.getBufferData("NumpyBufferTest", "unsignedIntegerBuffer")
floatArray = pypackagesutil.getBufferData("NumpyBufferTest", "floatBuffer")

l = intArray.tolist()
for ind,val in enumerate(l):	
    intArray[ind] = val+1
print intArray

l = uintArray.tolist()
for ind, val in enumerate(l):
	uintArray[ind] = val+1
print uintArray

l = floatArray.tolist()
for ind, val in enumerate(l):
	floatArray[ind] = val + 1.0
print floatArray