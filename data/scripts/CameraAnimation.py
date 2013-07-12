# Inviwo Python script 
import inviwo 
import math 

d = 5
for i in range(0,360):
   r = (2 * 3.14 * i) / 360
   x = d*math.sin(r)
   z = -d*math.cos(r)
   inviwo.setPropertyValue("EntryExitPoints","camera",((x,0,z),(0,0,0),(0,1,0)))
