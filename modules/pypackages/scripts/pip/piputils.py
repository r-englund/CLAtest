import pip
import sys
import inviwo_internal

def pipSearch(packageName):
    packageFound = "success"
    packageNotFound = "failed"
    InviwoStdOut = sys.stdout.stop()    
    try:
        status=pip.main(['search',packageName])
        if status==0:
            InviwoStdOut.write(packageFound)
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
        status=pip.main(['install',packageName])
        if status==0:
            InviwoStdOut.write(packageInstalled)
        else:
            InviwoStdOut.write(packageNotInstalled)
    except:
        InviwoStdOut.write(packageNotInstalled)
    InviwoStdOut.start()