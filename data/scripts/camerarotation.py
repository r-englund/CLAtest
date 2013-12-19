# Inviwo Python script 
import inviwo 
import math 

d = 5
steps = 120
for i in range(0, steps):
   r = (2 * 3.14 * i) / steps
   x = d*math.sin(r)
   z = -d*math.cos(r)
   inviwo.setPropertyValue("EntryExitPoints","camera",((x,3,z),(0,0,0),(0,1,0)))


for i in range(0, steps):
   r = (2 * 3.14 * i) / (3*steps)
   x = 1.0*math.sin(r)
   z = 1.0*math.cos(r)
   inviwo.setCameraUp("EntryExitPoints","camera",(x,z,0))

