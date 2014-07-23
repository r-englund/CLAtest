import pip
import sys
from cStringIO import StringIO
import inviwo_internal

def pipList(packageName):
    packageFound = "success"
    packageNotFound = "failed"
    InviwoStdOut = sys.stdout.stop()
    sys.stdout = strStdout = StringIO() 
    try:
        status=pip.main(['list'])
        if status==0:                    	
            packageList = strStdout.getvalue()            
            if packageList.find(packageName)!=-1:
                InviwoStdOut.write(packageFound)
            else:                
                InviwoStdOut.write(packageNotFound)
        else:
            InviwoStdOut.write(packageNotFound)
    except:
        InviwoStdOut.write(packageNotFound)
    InviwoStdOut.start()

def pipInstall(packageName):
    packageInstalled = "success"
    packageNotInstalled = "failed"
    InviwoStdOut = sys.stdout.stop()    
    try:
        status=pip.main(['install', packageName])
        if status==0:
            InviwoStdOut.write(packageInstalled)
        else:
            InviwoStdOut.write(packageNotInstalled)
    except:
        InviwoStdOut.write(packageNotInstalled)
    InviwoStdOut.start()

def pipUnInstall(packageName):
    packageUnInstalled = "success"
    packageNotUnInstalled = "failed"
    InviwoStdOut = sys.stdout.stop()    
    try:
        status=pip.main(['uninstall',packageName])
        if status==0:
            InviwoStdOut.write(packageUnInstalled)
        else:
            InviwoStdOut.write(packageNotUnInstalled)
    except:
        InviwoStdOut.write(packageNotUnInstalled)
    InviwoStdOut.start()

def pipUpgrade(packageName):
    packageUpgraded = "success"
    packageNotUpgraded = "failed"
    InviwoStdOut = sys.stdout.stop()
    #todo:implement later
    InviwoStdOut.write(packageNotUpgraded)    
    InviwoStdOut.start()