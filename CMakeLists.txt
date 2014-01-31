# Interactive Visualization Workshop (Inviwo)
if(DESIRED_QT_VERSION MATCHES 5)
cmake_minimum_required(VERSION 2.8.11)
cmake_policy(VERSION 2.8.11)
else()
cmake_minimum_required(VERSION 2.8.4)
cmake_policy(VERSION 2.8.4)
endif()
project(inviwo-projects)
set(IVW_VERSION 0.6b)
include(cmake/globalconfig.cmake)

#--------------------------------------------------------------------
# Default setup of options for the build

# Modules
build_module(Base)
build_module(BaseGL)
build_module(OpenGL)
build_module(OpenGLQt)

# Applications/Qt
option(IVW_QT_APPLICATION "Build Inviwo Qt Application" ON)
option(IVW_QT_EDITOR "Build Inviwo Qt Editor" ON)
option(IVW_QT_WIDGETS "Build Inviwo Qt Widgets" ON)

option(IVW_TINY_GLUT_APPLICATION "Build Inviwo Tiny GLUT Application" OFF)
build_module_dependency(GLUT IVW_TINY_GLUT_APPLICATION)

option(IVW_UNIT_TEST_APPLICATION "Build Inviwo Unit tests Application" OFF)

#--------------------------------------------------------------------
# Add subdirectories

add_subdirectory(ext/ticpp)
add_subdirectory(ext/tinythread) 
add_subdirectory(ext/sigar)
add_subdirectory(ext/freeimage)
add_subdirectory(src/core)
add_subdirectory(src/qt)
add_subdirectory(modules)
add_subdirectory(apps)
