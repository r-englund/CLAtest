# Inviwo Python script 
import inviwo
import inviwoqt 
import os
import time

start = time.clock()


workspaceFolder =  inviwo.getDataPath() + "workspaces/tests/";
imgFolder =   inviwo.getDataPath() + "tests/allworkspaceloadtest/outputimages/";

def workspace(f,ws):
   inviwoqt.loadWorkspace(f)
   numCanvas = inviwo.canvasCount()
   for i in range(0,numCanvas):
      inviwo.snapshotCanvas(i,imgFolder + ws + str(i) + ".png") 

print "Mem at start: " + str(inviwo.getMemoryUsage() / (1024.0*1024.0)) + " M"

for filename in os.listdir(workspaceFolder):
    if filename.endswith(".inv"):
        if filename.startswith("vrn"):
            continue
        if "gromac" in filename:
            continue
        print "Loading " + filename + ", " + str(inviwo.getMemoryUsage() / (1024.0*1024.0)) + " M"
        workspace(workspaceFolder + filename,filename)
        print " "
    
end = time.clock()

print "Mem at end: " + str(inviwo.getMemoryUsage() / (1024.0*1024.0)) + " M"
print str(end - start) + " sec"