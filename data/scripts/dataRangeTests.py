# Inviwo Python script 
import inviwo 
import inviwoqt 
import os

inviwo.info() 
inviwoqt.info() 

for f in os.listdir(inviwo.getVolumePath()+os.sep+"testdata"):
    if ".dat" in f:
        print(f)
        inviwo.setPropertyValue("VolumeSource", "filename",inviwo.getVolumePath()+os.sep+"testdata" + os.sep + f)
        inviwo.snapshot(inviwo.getImagePath()+os.sep+f+".jpg")