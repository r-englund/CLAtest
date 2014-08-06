import pip
from setuptools.command import easy_install
from collections import defaultdict
import sys
from cStringIO import StringIO
import urllib2
import subprocess as sp
import imp
import inviwo_internal

#hard coded urls
#TODO: Use web scraping to get updated versions or host it in ftp
WinPythonInstallerURL = 'http://www.lfd.uci.edu/~gohlke/pythonlibs/'
RAND_VAR = 'ekvkz8ci/' #this is dynamic???
WinPythonInstallerURL = WinPythonInstallerURL + RAND_VAR

unofficialPythonPackages_27 = defaultdict(list)
unofficialPythonPackages_27.clear()

#numpy
unofficialPythonPackages_27['numpy'].append(WinPythonInstallerURL+'numpy-MKL-1.9.0b2.win-amd64-py2.7.exe')
unofficialPythonPackages_27['numpy'].append(WinPythonInstallerURL+'numpy-MKL-1.9.0b2.win32-py2.7.exe')

#matplotlib & dependencies (dateutil, pytz, pyparsing, six)
unofficialPythonPackages_27['matplotlib'].append(WinPythonInstallerURL+'matplotlib-1.4.0rc2.win-amd64-py2.7.exe')
unofficialPythonPackages_27['matplotlib'].append(WinPythonInstallerURL+'matplotlib-1.4.0rc2.win32-py2.7.exe')
#dateutil
unofficialPythonPackages_27['python-dateutil'].append(WinPythonInstallerURL+'python-dateutil-2.2.win-amd64-py2.7.exe')
unofficialPythonPackages_27['python-dateutil'].append(WinPythonInstallerURL+'python-dateutil-2.2.win32-py2.7.exe')
#pytz
unofficialPythonPackages_27['pytz'].append(WinPythonInstallerURL+'pytz-2014.4.win-amd64-py2.7.exe')
unofficialPythonPackages_27['pytz'].append(WinPythonInstallerURL+'pytz-2014.4.win-amd64-py2.7.exe')
#pyparsing
unofficialPythonPackages_27['pyparsing'].append(WinPythonInstallerURL+'pyparsing-2.0.2.win-amd64-py2.7.exe')
unofficialPythonPackages_27['pyparsing'].append(WinPythonInstallerURL+'pyparsing-2.0.2.win-amd64-py2.7.exe')
#six
unofficialPythonPackages_27['six'].append(WinPythonInstallerURL+'six-1.7.3.win-amd64-py2.7.exe')
unofficialPythonPackages_27['six'].append(WinPythonInstallerURL+'six-1.7.3.win32-py2.7.exe')

#scikit-learn & #scikit-image
unofficialPythonPackages_27['scikit-learn'].append(WinPythonInstallerURL+'scikit-learn-0.15.1.win-amd64-py2.7.exe')
unofficialPythonPackages_27['scikit-learn'].append(WinPythonInstallerURL+'scikit-learn-0.15.1.win32-py2.7.exe')
unofficialPythonPackages_27['scikit-image'].append(WinPythonInstallerURL+'scikit-image-0.10.1.win-amd64-py2.7.exe')
unofficialPythonPackages_27['scikit-image'].append(WinPythonInstallerURL+'scikit-image-0.10.1.win32-py2.7.exe')

#pycuda & dependencies
unofficialPythonPackages_27['pycuda'].append(WinPythonInstallerURL+'pycuda-2013.1.1-cuda-6.0.37.win-amd64-py2.7.exe')
unofficialPythonPackages_27['pycuda'].append(WinPythonInstallerURL+'pycuda-2013.1.1-cuda-6.0.37.win32-py2.7.exe')
#pytools
unofficialPythonPackages_27['pytools'].append(WinPythonInstallerURL+'pytools-2014.3.win-amd64-py2.7.exe')
unofficialPythonPackages_27['pytools'].append(WinPythonInstallerURL+'pytools-2014.3.win32-py2.7.exe')



def is64bit():
    return (sys.maxsize > 2**32)

def isPlatformWindows():
    import platform
    if str(platform.system())=='Windows' or str(platform.system())=='windows': return True
    return False

def isModuleAvailable(moduleName):
    moduleAvailable = "success"
    moduleNotAvailable = "failed"
    InviwoStdOut = sys.stdout.stop()
    sys.stdout = strStdout = StringIO()     
    try:
        imp.find_module(moduleName)
        InviwoStdOut.write(moduleAvailable)
    except ImportError:
        InviwoStdOut.write(moduleNotAvailable)
    InviwoStdOut.start()


def pipPackageInfo(packageName):
    packageNotFound = 'Name: '+packageName+'\n'
    packageNotFound = packageNotFound + 'Version: '+'0.0.0'+'\n'
    packageNotFound = packageNotFound + 'Location: '+'-'+'\n'
    packageNotFound = packageNotFound + 'Requires: '+'-'+'\n'

    InviwoStdOut = sys.stdout.stop()
    sys.stdout = strStdout = StringIO() 
    try:
        status=pip.main(['show', packageName])
        if status==0:                       
            packageInfo = strStdout.getvalue()
            if packageInfo=='':
                InviwoStdOut.write(packageNotFound)
            else:
                InviwoStdOut.write(packageInfo)
        else:
            InviwoStdOut.write(packageNotFound)
    except:
        InviwoStdOut.write(packageNotFound)
    InviwoStdOut.start()

def pipPackageVersion(packageName):    
    packageNotFound = '0.0.0'

    InviwoStdOut = sys.stdout.stop()
    sys.stdout = strStdout = StringIO() 
    try:
        status=pip.main(['show', packageName])
        if status==0:                       
            packageInfo = strStdout.getvalue()
            if packageInfo=='':
                InviwoStdOut.write(packageNotFound)
            else:
                start = packageInfo.find('Version:') + 8
                end = packageInfo.find('\n', start)
                version=packageInfo[start:end]
                InviwoStdOut.write(version)
        else:
            InviwoStdOut.write(packageNotFound)
    except:
        InviwoStdOut.write(packageNotFound)
    InviwoStdOut.start()

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

def easyInstall(packageName):
    global unofficialPythonPackages_27
    packageInstalled = "success"
    packageNotInstalled = "failed"
    InviwoStdOut = sys.stdout.stop()    
    try:
        binaryURL=None

        if packageName in unofficialPythonPackages_27:
            if is64bit()==True:
                binaryURL = unofficialPythonPackages_27[packageName][0]
            else:
                binaryURL = unofficialPythonPackages_27[packageName][1]       

            #fetch
            req = urllib2.Request(binaryURL, headers={ 'User-Agent': 'Mozilla/5.0' })
            content = urllib2.urlopen(req).read()

            #save to local disk
            from os.path import expanduser
            tempFile = str(expanduser("~")) + '/' + packageName + '.exe'

            #InviwoStdOut.write(tempFile)
            with open(tempFile, 'wb') as output:
                output.write(content)

            #status=easy_install.main( ['-U',binaryURL] )            
            process = sp.Popen(tempFile, shell=True)
            process.wait()

            if status==0:
                InviwoStdOut.write(packageInstalled)
            else:
                InviwoStdOut.write(packageNotInstalled)
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

def installPyPackage(packageName):
    #on win32, install from unofficial binaries using easy_install
    #http://www.lfd.uci.edu/~gohlke/pythonlibs/
    #other platform, install from pypi server using pip
    if isPlatformWindows():
        easyInstall(packageName)
    else:
        pipInstall(packageName)
    