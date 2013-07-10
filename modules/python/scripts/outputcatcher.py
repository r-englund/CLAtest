# internal helper script used to redirect sys.stdout/sys.stderr
# to the C++ PythonModule.
import sys
import inviwo_internal

class OutputCatcherStdout:
    def write(self, string):
        inviwo_internal.ivwPrint(string, 0)

class OutputCatcherStderr:
    def write(self, string):
        inviwo_internal.ivwPrint(string, 1)
        
sys.stdout = OutputCatcherStdout()  
sys.stderr = OutputCatcherStderr()  
