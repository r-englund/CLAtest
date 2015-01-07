# Inviwo Python script 
import inviwo
import inviwoqt 
import os
import time

########## Enable this for comparing with reference image
########## Requires urllib2, base64, getpass
########## Remember to set username and password


def getUser():
    user = inviwoqt.prompt('',"Enter user name: ")
    return user.rstrip( '\n' )    
    
def getPassword():    
    pwd = inviwoqt.prompt('',"Enter password: ")
    return pwd.rstrip( '\n' )


refFolder =   inviwo.getDataPath() + "tests/allworkspaceloadtest/referenceimages/";
import sys
import filecmp
import getpass
from sys import stderr
usr = getUser()
pwd = getPassword()
sys.path.append(inviwo.getDataPath() + "tests/allworkspaceloadtest")
import inviworeferencescrap as ivwRef
ivwRef.setInviwoAuthentication(usr,pwd)

def downloadAndCompareReferenceImage(ws,i):
   ivwRef.downloadInviwoReferenceImage(ws,refFolder + ws + str(i+1)+ ".png",i+1)
   comparisionStatus = filecmp.cmp(imgFolder + ws + str(i+1)+ ".png",refFolder + ws + str(i+1)+ ".png")
   if comparisionStatus==False:
      print(ws + ' canvassnapshot-' + str(i+1) + ' FAILED')



########## Enable this for comparing with reference image

start = time.clock()


workspaceFolder =  inviwo.getDataPath() + "workspaces/tests/";
imgFolder =   inviwo.getDataPath() + "tests/allworkspaceloadtest/outputimages/";
def workspace(f,ws):
   inviwoqt.loadWorkspace(f)
   numCanvas = inviwo.canvasCount()
   for i in range(0,numCanvas):
      inviwo.snapshotCanvas(i,imgFolder + ws + str(i+1) + ".png") 
      ########## Enable this for comparing with reference image
      downloadAndCompareReferenceImage(ws,i)

print("Mem at start: " + str(inviwo.getMemoryUsage() / (1024.0*1024.0)) + " M")
print(workspaceFolder)
for filename in os.listdir(workspaceFolder):
    if filename.endswith(".inv"):
        if filename.startswith("vrn"):
            continue
        if "gromac" in filename:
            continue
        print("Loading " + filename + ", " + str(inviwo.getMemoryUsage() / (1024.0*1024.0)) + " M")
        workspace(workspaceFolder + filename,filename)
        print(" ")
    
end = time.clock()

print("Mem at end: " + str(inviwo.getMemoryUsage() / (1024.0*1024.0)) + " M")
print(str(end - start) + " sec")