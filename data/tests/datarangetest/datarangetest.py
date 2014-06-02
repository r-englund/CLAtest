# Inviwo Python script 
import inviwo 
import inviwoqt 
import os

path = inviwo.getDataPath()+os.sep+"tests"+os.sep+"datarangetest"

for f in os.listdir(path+os.sep+"datasets"):
    if ".dat" in f:
        print(f)
        inviwo.setPropertyValue("VolumeSource", 
                                              "filename",
                                              path + os.sep + "datasets" + os.sep + f)
        inviwo.snapshot(path + os.sep + "testdata" + os.sep + f + ".jpg")