## Inviwo - Interactive Visualization Workshop

Inviwo is a software framework for rapid visualization prototyping.
Below follow some general information about the framework:

 - Freely available under the Simplified BSD License.
 - Cross-platform and runs on Windows, Linux and Mac OS X.
 - Easily extendable through inclusion of external modules and projects.

### Core
 - The core is written in only C/C++, with minor dependencies.
 
### Modules
 - Modern graphics programming is supported through provided OpenGL (> 3.2 Core) modules.
 - Parallel computing on multiple platforms (GPU/CPU) is supported through OpenCL (> 1.0) modules.
 - Python (> 3.2) scripting and computation is supported through provided modules.

 ### GUI
 - The primary GUI is based on Qt (Recommended > 5.2, Supported > 4.8).
 - A minimal application is available, utilizing GLFW 3 for multiple window and context management.
 
 ### Build system
 - The project and module configuration/generation is performed through CMake (> 2.8.11)
 - Inviwo has been compiled in Visual Studio (2008/2012/2013) 