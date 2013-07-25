# Interactive Visualization Workshop (Inviwo)
cmake_minimum_required(VERSION 2.8.10)
project(inviwo-projects)
set(IVW_VERSION 0.1)
include(cmake/globalconfig.cmake)

#--------------------------------------------------------------------
# Default setup of options for the build

# Modules
build_module(Base)
build_module(OpenGL)
build_module(OpenglQt)

# Applications/Qt
option(IVW_QT_APPLICATION "Build Inviwo Qt Application" ON)
option(IVW_QT_EDITOR "Build Inviwo Qt Editor" ON)
option(IVW_QT_WIDGETS "Build Inviwo Qt Widgets" ON)

option(IVW_TINY_GLUT_APPLICATION "Build Inviwo Tiny GLUT Application" OFF)
build_module_dependency(GLUT IVW_TINY_GLUT_APPLICATION)

option(IVW_DICOM_APPLICATION "Build DICOM Application" OFF)
build_module_dependency(DICOM IVW_DICOM_APPLICATION)


option(TEST_APPLICATIONS "Build All Test Application" OFF)

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